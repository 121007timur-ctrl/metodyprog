# TextFilter Testing

## Тест-кейсы (от команды)

[Тест-кейс и дефект в одной папке](client/test_case/test_case_client.xlsx) (Насыров Тимур)

[Тест-кейс и дефект в папке сервер](server/defect%20test/server_registration_db_test_case.xlsx) (Чистяков Аким)

## Диаграмма классов для Сервера

```mermaid
classDiagram
    class Server {
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

    class ClientSession {
        -int id
        -SOCKET socket
        -string login
        -bool isAuthenticated
        +send(string data) bool
        +receive() string
        +close() void
    }

    class Authorization {
        -string login
        -string passwordHash
        -bool isAuthenticated
        -string role
        +login(string login, string pass) bool
        +logout() void
        +hasRole(string role) bool
    }

    class Database {
        -string connectionString
        +connect() bool
        +disconnect() void
        +query(string sql) ResultSet
        +getUserByLogin(string login) User
        +saveUser(User user) bool
    }

    class User {
        +int id
        +string name
        +string email
        +string role
        +getStats() UserStats
    }

    class Admin {
        +getUserList() vector~User~
        +deleteUser(int userId) bool
        +getStatistics() Stats
    }

    Server --> ClientSession : manages
    Server --> Authorization : uses
    Server --> Database : uses
    Authorization --> User : creates
    Admin --|> User : inherits
    Admin --> Database : uses
