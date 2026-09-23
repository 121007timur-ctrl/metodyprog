#include "newton.h"
#include <QRandomGenerator>
#include <algorithm>
#include <cmath>

namespace {

/// Добавляет к записи многочлена слагаемое coef*x^power со знаком.
void appendTerm(QString& out, double coef, int power)
{
    if (coef == 0) return;

    const bool negative = coef < 0;
    const double absCoef = std::fabs(coef);

    if (out.isEmpty()) {
        if (negative) out += "-";
    } else {
        out += negative ? " - " : " + ";
    }

    const bool unit = (absCoef == 1.0);
    if (!unit || power == 0) out += QString::number(absCoef);

    if (power >= 1) out += "x";
    if (power >= 2) out += "^" + QString::number(power);
}

/// Число в ответе сервера: до 6 значащих цифр, без хвостовых нулей.
QString num(double v)
{
    if (std::fabs(v) < 1e-12) v = 0;
    return QString::number(v, 'g', 6);
}

} // namespace

double Polynomial::value(double x) const
{
    return ((c3 * x + c2) * x + c1) * x + c0;
}

double Polynomial::derivative(double x) const
{
    return (3 * c3 * x + 2 * c2) * x + c1;
}

QString Polynomial::toString() const
{
    QString s;
    appendTerm(s, c3, 3);
    appendTerm(s, c2, 2);
    appendTerm(s, c1, 1);
    appendTerm(s, c0, 0);
    return s.isEmpty() ? "0" : s;
}

QString Polynomial::derivativeToString() const
{
    QString s;
    appendTerm(s, 3 * c3, 2);
    appendTerm(s, 2 * c2, 1);
    appendTerm(s, c1, 0);
    return s.isEmpty() ? "0" : s;
}

NewtonStep newtonFirstStep(const Polynomial& f, double x0)
{
    return newtonFirstStep(f, x0, f.derivative(x0));
}

NewtonStep newtonFirstStep(const Polynomial& f, double x0, double dfx0)
{
    NewtonStep step;
    step.x0 = x0;
    step.fx0 = f.value(x0);
    step.dfx0 = dfx0;

    if (std::fabs(step.dfx0) < 1e-12) {
        step.error = "f'(x0) = 0: касательная параллельна оси OX, выберите другое x0";
        return step;
    }

    step.x1 = x0 - step.fx0 / step.dfx0;
    step.fx1 = f.value(step.x1);
    step.ok = true;
    return step;
}

bool parseNewtonArgs(const QStringList& args, Polynomial& f, double& x0, QString& error,
                     bool* hasDerivative, double* dfx0)
{
    if (args.size() != 5 && args.size() != 6) {
        error = "нужно 5 или 6 чисел: c3 c2 c1 c0 x0 [f'(x0)]";
        return false;
    }

    double v[6];
    for (int i = 0; i < args.size(); ++i) {
        QString s = args[i];
        s.replace(',', '.');
        bool ok = false;
        v[i] = s.toDouble(&ok);
        if (!ok || !std::isfinite(v[i])) {
            error = "\"" + args[i] + "\" не является числом";
            return false;
        }
    }

    f.c3 = v[0];
    f.c2 = v[1];
    f.c1 = v[2];
    f.c0 = v[3];
    x0 = v[4];

    const bool given = (args.size() == 6);
    if (hasDerivative) *hasDerivative = given;
    if (dfx0) *dfx0 = given ? v[5] : f.derivative(x0);
    return true;
}

QString handleNewtonCommand(const QStringList& args)
{
    Polynomial f;
    double x0 = 0;
    double dfx0 = 0;
    bool derivativeGiven = false;
    QString error;

    if (!parseNewtonArgs(args, f, x0, error, &derivativeGiven, &dfx0)) {
        return "\r\nNEWTON ERROR: " + error + "\r\n"
               "Формат: NEWTON c3 c2 c1 c0 x0 [f'(x0)]  (f(x) = c3*x^3 + c2*x^2 + c1*x + c0)\r\n";
    }

    const NewtonStep step = newtonFirstStep(f, x0, dfx0);
    if (!step.ok) {
        return "\r\nNEWTON ERROR: " + step.error + "\r\n";
    }

    QString r = "\r\n=== МЕТОД НЬЮТОНА (Вариант 4) ===\r\n";
    r += "f(x)  = " + f.toString() + "\r\n";
    r += "f'(x) = " + f.derivativeToString() + "\r\n";
    r += "x0 = " + num(step.x0) + "\r\n";
    r += "f(x0) = " + num(step.fx0) + "\r\n";
    if (derivativeGiven) {
        const double byFormula = f.derivative(x0);
        r += "f'(x0) = " + num(step.dfx0) + " (задано)\r\n";
        if (std::fabs(byFormula - step.dfx0) > 1e-9 * std::max(1.0, std::fabs(byFormula))) {
            r += "ВНИМАНИЕ: по формуле f'(x0) = " + num(byFormula) + ", заданное значение отличается\r\n";
        }
    } else {
        r += "f'(x0) = " + num(step.dfx0) + "\r\n";
    }
    r += "x1 = x0 - f(x0)/f'(x0) = " + num(step.x1) + "\r\n";
    r += "f(x1) = " + num(step.fx1) + "\r\n";
    r += "NEWTON OK x1=" + num(step.x1) + "\r\n";
    return r;
}

void generateNewtonTask(Polynomial& f, int& x0)
{
    QRandomGenerator* rnd = QRandomGenerator::global();
    do {
        f.c3 = rnd->bounded(0, 2);           // 0 или 1: квадратный или кубический
        f.c2 = rnd->bounded(-3, 4);
        f.c1 = rnd->bounded(-6, 7);
        f.c0 = rnd->bounded(-9, 10);
        if (f.c3 == 0 && f.c2 == 0) f.c2 = 1; // многочлен не ниже 2-й степени
        x0 = rnd->bounded(-4, 5);
    } while (std::fabs(f.derivative(x0)) < 1 || f.value(x0) == 0);
}
