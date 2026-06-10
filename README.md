## Тест-кейсы
[Тест-кейс и дефект в одной папке](client/test_case/test_case_client.xlsx)(Насыров Тимур)
[Тест-кейс и дефект в папке сервер](server/defect test/server_registration_db_test_case.xlsx)(Чистяков Аким).


## Диаграмма классов

```mermaid
classDiagram
    class Сервер {
        +int порт
        +запустить()
        +остановить()
        +обработатьЗапрос()
    }
    
    class Клиент {
        +string хост
        +int порт
        +подключиться()
        +отправитьДанные()
        +получитьДанные()
    }
    
    class СетевоеСоединение {
        -SOCKET сокет
        +отправить()
        +получить()
        +закрыть()
    }
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

### Статус
| Область | Статус |
|---------|--------|
| Меню + кнопки | ⬜ Ожидает сборки |
| Элементы списка | ⬜ Ожидает сборки |
| Фильтры | ⬜ Ожидает сборки |
| Сортировка | ⬜ Ожидает сборки |    
    Сервер --> СетевоеСоединение : управляет
    Клиент --> СетевоеСоединение : использует
