# metodyprog — документация кода {#mainpage}

Клиент-серверное приложение на Qt 6 (C++): регистрация и вход пользователей,
задания по численным методам и игра «Угадай число».

## Состав

| Часть | Точка входа | Основные классы и модули |
|-------|-------------|--------------------------|
| Сервер (`server/`) | `main.cpp` | MyTcpServer, Database, server_functions.h, newton.h |
| Клиент (`client/`) | `main.cpp` | AUTHregForm, MainWindow, ClientAPI, functionclient.h |
| Тесты (`tests/`) | `test_main.cpp` | TestNewton, TestDatabase, TestFunctionClient |

## Протокол

Клиент и сервер обмениваются текстовыми строками по TCP (порт 33333).
Полный список команд — в описании файла server_functions.h.

## Вариант 4 — метод Ньютона

Функция задаётся многочленом \f$f(x) = c_3x^3 + c_2x^2 + c_1x + c_0\f$.
Первое приближение:

\f[ x_1 = x_0 - \frac{f(x_0)}{f'(x_0)} \f]

- сервер: newton.h — newtonFirstStep(), handleNewtonCommand(), task4Handler();
- клиент: ClientAPI::newtonStep(), buildNewtonCommand(), parseNewtonResponse(),
  кнопки «Задание 4 (Ньютон)» и «Вариант 4: Ньютон» в MainWindow.

## Как собрать документацию

```
doxygen Doxyfile
```

Результат — `docs/html/index.html`. В GitHub Actions документация собирается
автоматически и прикладывается к запуску как артефакт `doxygen-docs`.
