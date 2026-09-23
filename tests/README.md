# Юнит-тесты (Qt Test)

Тесты проверяют настоящий код проекта: исходники сервера и клиента
подключаются в тестовый проект напрямую (`tests.pro`), моков нет.

| Набор | Что тестирует | Файл кода |
|-------|---------------|-----------|
| `TestNewton` | Вариант 4: многочлен, производная, шаг метода Ньютона, разбор команды `NEWTON`, генератор задания `TASK4` | `server/newton.cpp` |
| `TestDatabase` | Синглтон БД: регистрация, дубли логина/email, вход по логину и email, игровая статистика, топ игроков | `server/database.cpp` |
| `TestFunctionClient` | Клиент: формирование команды `NEWTON`, разбор ответа сервера, удаление меню из ответа, полный цикл клиент → сервер → клиент; варианты 1 и 16 | `client/functionclient.cpp` |

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
TestNewton:          Totals: 26 passed, 0 failed
TestDatabase:        Totals: 14 passed, 0 failed
TestFunctionClient:  Totals:  9 passed, 0 failed
```
