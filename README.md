# TextFilter Testing

## Test Cases (from team)

[Test case and defect in one folder](client/test_case/test_case_client.xlsx) (Насыров Тимур)

[Test case and defect in server folder](server/defect%20test/server_registration_db_test_case.xlsx) (Чистяков Аким)

## Class Diagram (client-server)

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

    class Client {
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

    class NetworkConnection {
        -SOCKET socket
        -string lastError
        +send(string data) int
        +receive() string
        +close() void
        +isValid() bool
        +getLastError() string
    }

    class Authorization {
        -string login
        -string passwordHash
        -bool isAuthenticated
        -string role
        +login(string login, string pass) bool
        +logout() void
        +hasRole(string requiredRole) bool
        +changePassword(string oldPass, string newPass) bool
    }

    class User {
        +int id
        +string name
        +string email
        +string role
        +getStats() UserStats
        +updateProfile() bool
    }

    class Admin {
        +getUserList() vector~User~
        +deleteUser(int userId) bool
        +getStatistics() Stats
        +setUserRole(int userId, string role) bool
    }

    class Filter {
        +string pattern
        +bool caseSensitive
        +apply(vector~string~ input) vector~string~
        +clear() void
        +isActive() bool
    }

    class Sorting {
        +sortAsc(vector~string~ input) vector~string~
        +sortDesc(vector~string~ input) vector~string~
        +sortWithFilter(vector~string~ input, Filter filter, bool asc) vector~string~
    }

    class Database {
        -string connectionString
        +connect() bool
        +disconnect() void
        +query(string sql) ResultSet
        +execute(string sql) bool
        +getUserByLogin(string login) User
        +saveUser(User user) bool
    }

    Server --> NetworkConnection : uses
    Server --> Database : works with
    Server --> Authorization : manages
    
    Client --> NetworkConnection : uses
    Client --> Authorization : contains
    
    Admin --|> User : inherits
    
    Client --> Filter : uses
    Client --> Sorting : uses
    
    Authorization --> User : creates
    Admin --> Database : manages
