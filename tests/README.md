# Юнит-тесты (Qt Test)

Тесты проверяют настоящий код проекта: исходники сервера и клиента
подключаются в тестовый проект напрямую (`tests.pro`), моков нет.

| Набор | Тестов | Что тестирует | Файл кода |
|-------|-------:|---------------|-----------|
| `TestNewton` | 27 | Вариант 4: многочлен, производная, шаг метода Ньютона (в том числе с заданной f'(x0)), разбор команды `NEWTON`, генератор задания `TASK4` | `server/newton.cpp` |
| `TestDatabase` | 12 | Синглтон БД: регистрация, дубли логина/email, вход по логину и email, игровая статистика, топ игроков | `server/database.cpp` |
| `TestFunctionClient` | 7 | Клиент: формирование команды `NEWTON`, разбор ответа сервера, удаление меню из ответа, полный цикл клиент → сервер → клиент; варианты 1 и 16 | `client/functionclient.cpp` |

## Запуск

### Qt Creator
Открыть `tests/tests.pro` → Build → Run. Результат — во вкладке «Вывод приложения».

### Командная строка (Windows, MinGW)
```bat
mkdir build-tests && cd build-tests
qmake ..\tests\tests.pro
mingw32-make
release\unit_tests.exe
```

### Linux
```bash
mkdir build-tests && cd build-tests
qmake ../tests/tests.pro && make
./unit_tests
```

Код возврата `0` — все тесты прошли, `1` — есть упавшие.
Если задать переменную `TEST_LOG_DIR`, отчёт каждого набора дополнительно
сохранится в файл `<TEST_LOG_DIR>/<Набор>.txt`.

## CI
Тесты запускаются автоматически в GitHub Actions
(`.github/workflows/tests.yml`) при каждом push и pull request.

## Результат последнего запуска

```
TestNewton:          Totals: 29 passed, 0 failed   # 27 тестов (с учётом строк _data) + initTestCase/cleanupTestCase
TestDatabase:        Totals: 14 passed, 0 failed   # 12 тестов + 2 служебных
TestFunctionClient:  Totals:  9 passed, 0 failed   #  7 тестов + 2 служебных
```
