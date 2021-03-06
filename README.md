# ProHex - Advanced HEX editor

- **[English](#english)**
- **[Русский](#russian)**
- **[Screenshots/Скриншоты](#screenshots)**

<a name="english"></a>

## Small English Section

ProHex is a crossplatform multilanguage modular HEX editor written in C++ and Qt5.

Supported languages:
- English
- Russian
- Ukrainian

### Features

- Create, view and edit binary and text files
- English, Russian and Ukrainian language support
- File analysis (type, size in bytes, additional. information)
- Convertation between 16 numerical representations (Converter module)
- XOR encryption
- Dec, Bin, HEX converter (Converter module)
- ASCII table for system fonts (ASCIITable module)
- View and find ASCII strings in binary files (Strings module)
- Support for Windows, Linux and MacOS
- Find and replace text or bytes in the file (Finder module)
- Modularity (every module is an independent window and can be moved in the different screen)
- Light and dark color schemes
- Calculate MD5 hash of file

<a name="russian"></a>

## Русская Секция

ProHex - это кроссплатформенный мультиязычный модульный HEX редактор, написанный на C++ с использованием Qt5.

Поддерживаемые языки:
- Анлийский
- Русский
- Украинский

### Особенности

- Создание, просмотр и редактирование бинарных и текстовых файлов
- Доступность на русском, английском и украинском языках
- Анализ файла (тип, размер в байтах, доп. информация)
- Конвертер между 16 числовыми представлениями (модуль Converter)
- XOR шифрование
- Конвертер между двоичной, десятичной и шестнадцатеричной системами числения (модуль Converter)
- ASCII таблица с возможностью выбора шрифта (модуль ASCIITable)
- Просмотр и поиск текстовых данных в бинарных файлах (модуль Strings)
- Поддержка Windows, Linux, MacOS
- Поиск и замена текста или байтов в файле (модуль Finder)
- Модульность (каждый модуль представляет отдельное окно, которое можно вынести на отдельный монитор)
- Светлая и тёмная цветовые схемы
- Вычисление MD5 хеша файла (контрольной суммы)

### Описание пунктов меню

- Файл - Включает в себя все пункты, которые касаются работы с файлами.
  - Новый (Ctrl + N) - Создать новый файл, содержащий один нуль-байт.
  - Открыть (Ctrl + O) - Открывает диалоговое окно выбора файла, после чего файл откроется в областе просмотра.
  - Сохранить (Ctrl + S) - Сохраняет изменения в открытом файле или открывает диалоговое окно сохранения файла, если был создан новый файл.
  - Сохранить как… (Ctrl + Shift + S) - Открывает диалоговое окно сохранения файла. После этого, пункт “Сохранить” будет сохранять изменения в этот файл.
  - Сохранить копию (Ctrl + Alt + S) - Аналогично пункту “Сохранить как…”, но дальнейшнее сохранение будет проводится в текущий файл.
  - Выход (Ctrl + Q) - Предлагает сохранить изменения и завершает работу программы.
- Правка - Включает в себя все пункты меню, которые касаются редактирования файла.
  - История
    - Отмена (Ctrl + Z) - Отменяет последнее действие.
    - Повтор (Ctrl + R, Ctrl + Shift + Z) - Повторяет последнеюю отмену действия.
    - Открыть историю (Ctrl + Shift + H) - Открывает окно со списком правок в файле с возможностью вернуться на любую из них.
  - Поиск
    - Поиск (Ctrl + F) - Открывает окно поиска (модуль Finder).
    - Замена (Ctrl + H) - Открывает окно поиска со включённой возможностью замены (модуль Finder).
    - Искать далее (Ctrl + Shift + N) — Продолжить поиск вперёд, начиная с последней позиции.
    - Искать ранее (Ctrl + Shift + P) — Продолжить поиск назад, начиная с последней позиции.
  - Навигация
    - Перейти на (Ctrl + G) - Перемещает курсор на указанную позицию (в HEX), относительно начала файла.
  - Прочее
    - XOR шифрование (Ctrl + Shift + X) — Производит XOR шифрование открытого файла относительно указанного ключа.
- Вид - Включает в себя все пункты, которые касаются настроек внешнего вида редактора.
  - Области просмотра - Позволяет выбрать области окна просмотра файла, которые будут отображаться.
  - Настройки
    - Настройки - Открывает окно настроек.
- Инструменты - Включает в себя все пункты меню, которые касаются других полезных инструментов и модулей редактора.
  - Информация про файл
    - Анализатор - Отображает основную информацию про открытый файл.
  - Полезные утилиты
    - Конвертер (Ctrl + Shift + C) - Открывает окно Dec/Hex/Bin конвертера с отображением численных представлений байта на текущей позиции курсора (модуль Converter).
    - Строки (Ctrl + Shift + G) - Открывает окно, содержащее все читаемые строковые данные в открытом файле с возможностью поиска (модуль Strings).
    - ASCII Таблица (Ctrl + Shift + A) - Открывает окно, содержащую таблицу ASCII символов с возможностью просмотра через различные шрифты *и кодировки (не реализовано)* (модуль ASCIITable).
    - MD5 Хеш (Ctrl + Shift + M) — Отображает контрольную сумму открытого файла по алгоритму MD5.

### Клавиши управления в окне просмотра файла

- Клавиши навигации
  - Стрелки клавиатуры - Перемещение на один символ в соответствующем направлении.
  - PgUp - Перемещение на одну страницу вверх.
  - PgDown - Перемещение на одну страницу вниз.
  - Home - Перемещение в начало строки.
  - End - Перемещение в конец строки.
  - Ctrl + Home - Перемещение в начало файла.
  - Ctrl + End - Перемещение в конец файла.
- Клавиши выделения
  - Shift + Клавиши навигации - Выделение.
  - Ctrl + A - Выделить всё.
- Прочие клавиши
  - Ctrl + C - Скопировать выделенные байты в буфер обмена.
  - Tab - Переключение между HEX и ASCII секциями области просмотра.
  - Insert - Переключение между режимами вставки и замены.
- Действия с мышью
  - Нажатие левой клавиши мыши - Активация соответствующей секции окна просмотра и перемещение файлового указателя на позицию клика.
  - Перемещение мыши с зажатой левой клавишей - Выделение фрагмента файла в шестнадцатеричном или текстовом виде (зависит от секции окна просмотра, в которой происходит выделение).


<a name="screenshots"></a>

# Screenshots/Скриншоты

![All modules of ProHex](README_MD/modules.png)
- All modules of ProHex
- Все модули ProHex

![Translations of ProHex](README_MD/translations.png)
- Translations of ProHex
- Переводы ProHex

![Uncommon using of Finder module](README_MD/uncommonsearch.gif)
- Uncommon using of Finder module to convert the string to the byte sequence
- Нестандартное использование модуля Finder для преобразования строки в последовательность байтов

![Uncommon using of Converter module](README_MD/uncommonconverter.gif)
- Uncommon using of Converter module to get symbol code
- Нестандартное использование модуля Converter для получения кода символа

![Uncommon using of ASCIITable module](README_MD/uncommonascii.gif)
- Uncommon using of ASCIITable module to view installed fonts
- Нестандартное использование модуля ASCIITable просмотра шрифтов

![Strings module](README_MD/strings.png)
- Search text data in the binary file using Strings module
- Поиск текстовых данных в бинарных файлах с помощью модуля Strings
