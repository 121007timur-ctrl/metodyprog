# TextFilter Testing

## Тест-кейсы (от команды)

[Тест-кейс и дефект в одной папке](client/test_case/test_case_client.xlsx) (Насыров Тимур)

[Тест-кейс и дефект в папке сервер](server/defect%20test/server_registration_db_test_case.xlsx) (Чистяков Аким)

## Диаграмма классов (клиент-сервер)

```mermaid
classDiagram
    class Сервер {
        -int port
        -bool isRunning
        -SOCKET serverSocket
        -vector~ClientSession~ clients
        +Server(int port)
        +~Server()
        +start() bool
        +stop() void
        +handleRequest(Request req) Response
        +broadcastMessage(string msg) void
        #acceptConnection() void
        #processClient(int clientId) void
    }

    class Клиент {
        -string host
        -int port
        -SOCKET clientSocket
        -bool isConnected
        +Client(string host, int port)
        +~Client()
        +connect() bool
        +disconnect() void
        +sendData(string data) bool
        +receiveData() string
        +authenticate(string login, string pass) bool
    }

    class СетевоеСоединение {
        -SOCKET socket
        -string lastError
        +send(string data) int
        +receive() string
        +close() void
        +isValid() bool
        +getLastError() string
    }

    class Авторизация {
        -string login
        -string passwordHash
        -bool isAuthenticated
        -string role
        +login(string login, string pass) bool
        +logout() void
        +hasRole(string requiredRole) bool
        +changePassword(string oldPass, string newPass) bool
    }

    class Пользователь {
        +int id
        +string name
        +string email
        +string role
        +getStats() UserStats
        +updateProfile() bool
    }

    class Админ {
        +getUserList() vector~User~
        +deleteUser(int userId) bool
        +getStatistics() Stats
        +setUserRole(int userId, string role) bool
    }

    class Фильтр {
        +string pattern
        +bool caseSensitive
        +apply(vector~string~ input) vector~string~
        +clear() void
        +isActive() bool
    }

    class Сортировка {
        +enum Order { ASC, DESC }
        +sort(vector~string~ input, Order order) vector~string~
        +sortWithFilter(vector~string~ input, Filter filter, Order order) vector~string~
    }

    class БД {
        -string connectionString
        +connect() bool
        +disconnect() void
        +query(string sql) ResultSet
        +execute(string sql) bool
        +getUserByLogin(string login) User
        +saveUser(User user) bool
    }

    Сервер --> СетевоеСоединение : использует
    Сервер --> БД : работает с
    Сервер --> Авторизация : управляет
    
    Клиент --> СетевоеСоединение : использует
    Клиент --> Авторизация : содержит
    
    Админ --|> Пользователь : наследует
    
    Клиент --> Фильтр : использует
    Клиент --> Сортировка : использует
    
    Авторизация --> Пользователь : создаёт
    Админ --> БД : управляет
```

### Легенда
| Стрелка | Значение |
|---------|----------|
| `-->` | Использует (ассоциация) |
| `--|>` | Наследование |
| `*--` | Композиция |
| `o--` | Агрегация |

## Тестирование (Аслан Далаков)

### Тест-кейсы (Markdown)
- [01_menu_test_cases.md](test-cases/01_menu_test_cases.md)
- [02_list_elements_test_cases.md](test-cases/02_list_elements_test_cases.md)
- [03_filters_test_cases.md](test-cases/03_filters_test_cases.md)
- [04_sorting_test_cases.md](test-cases/04_sorting_test_cases.md)

### Найденные баги (Issues)

| № | Название | Приоритет |
|---|----------|-----------|
| [#1](https://github.com/121007timur-ctrl/metodyprog/issues/1) | При применении фильтра пропадают все элементы списка | Critical |
| [#2](https://github.com/121007timur-ctrl/metodyprog/issues/2) | Сортировка по убыванию не меняет порядок элементов | Major |
| [#3](https://github.com/121007timur-ctrl/metodyprog/issues/3) | Горячая клавиша Ctrl+S не сохраняет файл | Minor |

### Статус тестирования

| Область | Статус |
|---------|--------|
| Меню + кнопки | ⬜ Ожидает сборки |
| Элементы списка | ⬜ Ожидает сборки |
| Фильтры | ⬜ Ожидает сборки |
| Сортировка | ⬜ Ожидает сборки |
