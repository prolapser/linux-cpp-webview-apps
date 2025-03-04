# linux-cpp-webview-apps
создание нативных desktop-приложений из любого веб-сайта или веб-приложения на WebKit2GTK с блокировкой рекламы и портативными данными.

## webview приложения из любого URL

### особенности
- очень малый размер: около 140 кб для каждого приложения
- малое потребление RAM (в 5-7 раз меньше чем на Electron)
- в комплекте имеется блокировщик рекламы и трекеров
- используется системный webview на WebKit
- все данные приложений хранятся рядом с исполняемым файлом в папке `data` и приложения могут быть переносимы
- фактически это просто инструмент для создания webview-обвязки для неограниченного количества url: просто редактируешь `config.mk` и устанавливаешь: `make install`, создавая множество приложений одним инструментом

### зависимости:

в системе должны быть установлен `webkit2gtk` (например: `sudo pacman -S webkit2gtk`, в других дистрибутивах название может быть другим, например `webkit2gtk-devel` или `libwebkit2gtk-4.1-dev`)

во время сборки используются динамические библиотеки: `gtk+-3.0 webkit2gtk-4.1 gio-2.0`

для блокировки рекламы и трекеров используется https://github.com/jun7/wyebadblock

поумолчанию правила блокировки берутся из https://easylist.to/easylist/easylist.txt

правила блокировки хранятся в `~/.config/wyebadblock/easylist.txt` и при желании в этом файл можно поместить любые другие правила (например из uBlock Origin или AdGuard)

### как использовать
- подготовка:
  * склонировать это репо:

    `git clone --depth 1 --single-branch https://github.com/prolapser/linux-cpp-webview-apps && cd linux-cpp-webview-apps`

    или скачать и распаковать архив:

    `wget https://github.com/prolapser/linux-cpp-webview-apps/archive/refs/heads/main.zip -O linux-cpp-webview-apps.zip && unzip linux-cpp-webview-apps.zip && rm linux-cpp-webview-apps.zip && mv linux-cpp-webview-apps-main linux-cpp-webview-apps && cd linux-cpp-webview-apps`
  
    после успешного клонирования или распаковки архива будет произведен переход в каталог `linux-cpp-webview-apps`

  * отредактировать файл конфига:

    `nano config.mk`

    или любым удобным редактором, например:

    `leafpad config.mk` или `xed config.mk` и т.п.

    или в приложении по умолчанию если редактор неизвестен:

    `xdg-open config.mk`

    файл должен выглядеть примерно так:

    ```config
    APP_URL = https://ekzeget.ru/bible/bytie/glava-1/ # url web-приложения
    APP_NAME = Ekzeget # отображаемое название приложения латиницей, например "My Web App"
    VERSION = 1.0 # версия (не обязательно)
    CATEGORIES = Education;History;Literature # категории через точку с запятой: https://specifications.freedesktop.org/menu-spec/1.0/category-registry.html
    ICON_SRC = https://ekzeget.ru/frontassets/images/favicons/android-chrome-512x512.png  # или локальный путь до png-иконки
    ```

    ⚠️  ссылка `APP_URL` должна быть валидной https/http ссылкой на внешний web-ресурс или локальный адрес веб-приложения http://{ip}:{port}/

    ⚠️  имя программы `APP_NAME` нужно указывать латиницей, оно может содержать пробелы но не спецсимволы

    ⚠️  иконка `ICON_SRC` должны быть строго в PNG, указывать нужно прямую ссылку на файл или абсолютный путь до локального файла

    ⚠️  категории `CATEGORIES` нужно указывать пречисляя через `;` если их больше одной, они должны соответсвовать стандарным категориям freedesktop: https://specifications.freedesktop.org/menu-spec/1.0/category-registry.html

  * собрать:
    
    `make`

    после сборки можно открыть папку `build` и посмотреть все ли корректно, можно запустить приложение чтобы протестировать.

    если все нормально, можно переместить файлы куда хочется (приложение портативное если переносить всю его папку с бинарником, data и icon.png), или же выполнить установку в пункте ниже

  * установить:

    `make install`
 
    можно сразу вызывать `make install` вместо `make`, пропуская предыдущий пункт

    установка происходит в `~/.local/share/` а значок программы помещается в `~/.local/share/applications/`

    после этого иконка программы добавится в стандартное меню приложений.
    
  * удалить:
    
    `make uninstall`

---

### примеры содержимого `config.mk` для создания различных приложений


#### <img src="https://i.ibb.co/zMJxQ0n/sillytavern.png" height="32px"> SillyTavern
```
APP_URL = http://127.0.0.1:8000/
APP_NAME = SillyTavern
VERSION = 1.0
CATEGORIES = ArtificialIntelligence;RolePlaying;Chat
ICON_SRC = /home/user/SillyTavern/public/img/apple-icon-144x144.png
```

#### <img src="https://raw.githubusercontent.com/lobehub/lobe-chat/main/public/icons/icon-512x512.png" height="32px"> LobeChat
```
APP_URL = https://www.lobechat.xyz/
APP_NAME = LobeChat
VERSION = 1.0
CATEGORIES = ArtificialIntelligence;Education;Network;Chat
ICON_SRC = https://raw.githubusercontent.com/lobehub/lobe-chat/main/public/icons/icon-512x512.png
```

#### <img src="https://chatboxai.app/icon.png" height="32px"> ChatBox
```
APP_URL = https://web.chatboxai.app/
APP_NAME = ChatBox
VERSION = 1.0
CATEGORIES = ArtificialIntelligence;Education;Network;Chat
ICON_SRC = https://chatboxai.app/icon.png
```

#### <img src="https://i.ibb.co/RTjh7qbT/omnom.png" height="32px"> Cut the rope
```
APP_URL = https://app-289684.games.s3.yandex.net/289684/g9j7wfosgi55ciil3sikfv3nekt7do6j/index.html
APP_NAME = Cut the rope
VERSION = 1.0
CATEGORIES = Game;ArcadeGame
ICON_SRC = https://i.ibb.co/RTjh7qbT/omnom.png
```

#### <img src="https://i.ibb.co/TBqtD6xV/omnombubbles.png" height="32px"> Om Nom Bubbles
```
APP_URL = https://app-176736.games.s3.yandex.net/176736/irccequ210syrhavqvyvvjjpsw2b8izv/index.html
APP_NAME = Om Nom Bubble
VERSION = 1.0
CATEGORIES = Game;ArcadeGame
ICON_SRC = https://i.ibb.co/TBqtD6xV/omnombubbles.png
```

#### <img src="https://upload.wikimedia.org/wikipedia/commons/thumb/7/71/Spotify.png/512px-Spotify.png" height="32px"> Spotify
```
APP_URL = https://open.spotify.com/
APP_NAME = Spotify Player
VERSION = 1.0
CATEGORIES = Audio;Music;Player;AudioVideo
ICON_SRC = https://upload.wikimedia.org/wikipedia/commons/thumb/7/71/Spotify.png/512px-Spotify.png
```

#### <img src="https://audiomass.co/icon-app.png" height="32px"> Audiomass Audio Editor
```
APP_URL = https://audiomass.co
APP_NAME = Audiomass
VERSION = 1.0
CATEGORIES = Audio;AudioVideoEditing
ICON_SRC = https://audiomass.co/icon-app.png
```

#### <img src="https://i.ibb.co/Dg5TpVb5/clipchamp.png" height="32px"> ClipChamp
```
APP_URL = https://app.clipchamp.com
APP_NAME = ClipChamp
VERSION = 1.0
CATEGORIES = Video;AudioVideoEditing
ICON_SRC = https://i.ibb.co/Dg5TpVb5/clipchamp.png
```

#### <img src="https://www.photopea.com/promo/icon512.png" height="32px"> Photopea
```
APP_URL = https://fotopee-online.hf.space
APP_NAME = Photopea
VERSION = 1.0
CATEGORIES = Graphics;RasterGraphics;Photography
ICON_SRC = https://www.photopea.com/promo/icon512.png
```

#### <img src="https://raw.githubusercontent.com/viliusle/miniPaint/refs/heads/master/images/logo-colors.png" height="32px"> Mini Paint
```
APP_URL = https://viliusle.github.io/miniPaint/
APP_NAME = Mini Paint
VERSION = 1.0
CATEGORIES = Graphics;RasterGraphics
ICON_SRC = https://raw.githubusercontent.com/viliusle/miniPaint/refs/heads/master/images/logo-colors.png
```

#### <img src="https://jspaint.app/images/icons/512x512.png" height="32px"> JSPaint
```
APP_URL = https://jspaint.ap
APP_NAME = JSPaint
VERSION = 1.0
CATEGORIES = Graphics;RasterGraphics
ICON_SRC = https://jspaint.app/images/icons/512x512.png
```

#### <img src="https://res-1.cdn.office.net/todo/2595912_2.128.3/icons/todo-512.png" height="32px"> Microsoft To Do
```
APP_URL = https://to-do.live.com/tasks/today
APP_NAME = ToDo
VERSION = 1.0
CATEGORIES = Office;ProjectManagement
ICON_SRC = https://res-1.cdn.office.net/todo/2595912_2.128.3/icons/todo-512.png
```

#### <img src="https://www.gstatic.com/images/branding/product/2x/keep_2020q4_512dp.png" height="32px"> Google Keep
```
APP_URL = https://keep.google.com/u/0/
APP_NAME = Keep
VERSION = 1.0
CATEGORIES = Office;ProjectManagement
ICON_SRC = https://www.gstatic.com/images/branding/product/2x/keep_2020q4_512dp.png
```

#### <img src="https://is1-ssl.mzstatic.com/image/thumb/Purple211/v4/ef/21/25/ef21251f-d445-42f0-e990-27c3abc071b7/AppIcon-0-0-1x_U007epad-0-1-0-85-220.png/512x0w.png" height="32px"> Discord
```
APP_URL = https://discord.com/app
APP_NAME = Discord
VERSION = 1.0
CATEGORIES = Network;InstantMessaging;Chat
ICON_SRC = https://is1-ssl.mzstatic.com/image/thumb/Purple211/v4/ef/21/25/ef21251f-d445-42f0-e990-27c3abc071b7/AppIcon-0-0-1x_U007epad-0-1-0-85-220.png/512x0w.png
```

#### <img src="https://web.telegram.org/a/icon-512x512.png" height="32px"> Telegram
```
APP_URL = https://web.telegram.org/a/
APP_NAME = Telegram
VERSION = 1.0
CATEGORIES = Network;InstantMessaging;Chat
ICON_SRC = https://web.telegram.org/a/icon-512x512.png
```

#### <img src="https://abs.twimg.com/responsive-web/client-web/icon-ios.77d25eba.png" height="32px"> Twitter
```
APP_URL = https://x.com/home
APP_NAME = Twitter
VERSION = 1.0
CATEGORIES = Network;InstantMessaging;Chat
ICON_SRC = https://abs.twimg.com/responsive-web/client-web/icon-ios.77d25eba.png
```

#### <img src="https://i.postimg.cc/yYB0PTL2/mscopilot.png" height="32px"> Microsoft Copilot
```
APP_URL = https://copilot.microsoft.com
APP_NAME = Copilot
VERSION = 1.0
CATEGORIES = ArtificialIntelligence;Education;Network;Chatk
ICON_SRC = https://i.postimg.cc/yYB0PTL2/mscopilot.png
```

#### <img src="https://designer.microsoft.com/designer-logo-512.png" height="32px"> Microsoft Designer
```
APP_URL = https://designer.microsoft.com/image-creator
APP_NAME = Designer
VERSION = 1.0
CATEGORIES = Graphics;2DGraphics;RasterGraphics;ArtificialIntelligence
ICON_SRC = https://designer.microsoft.com/designer-logo-512.png
```

#### <img src="https://i.ibb.co/HLtkdtKh/duckai.png" height="32px"> DuckAI
```
APP_URL = http://www.duck.ai/
APP_NAME = DuckAI
VERSION = 1.0
CATEGORIES = ArtificialIntelligence;Education;Network;Chat
ICON_SRC = https://i.ibb.co/HLtkdtKh/duckai.png
```

#### <img src="https://translate.yandex.ru/icons/favicon.png" height="32px"> Яндекс Переводчик
```
APP_URL = https://translate.yandex.ru/
APP_NAME = YandexTranslate
VERSION = 1.0
CATEGORIES = Translation;Education
ICON_SRC = https://translate.yandex.ru/icons/favicon.png
```

#### <img src="https://vscode.dev/static/stable/code-512.png" height="32px"> Visual Studio Code
```
APP_URL = https://vscode.dev
APP_NAME = VSCode
VERSION = 1.0
CATEGORIES = Development;IDEl;WebDevelopment
ICON_SRC = https://vscode.dev/static/stable/code-512.png
```

#### <img src="https://i.ibb.co/FL8FM3tJ/notion.png" height="32px"> Notion
```  
APP_URL = https://www.notion.so
APP_NAME = Notion
VERSION = 1.0
CATEGORIES = Office;ProjectManagement
ICON_SRC = https://i.ibb.co/FL8FM3tJ/notion.png
```

#### <img src="https://raw.githubusercontent.com/iv-org/invidious/refs/heads/master/assets/android-chrome-512x512.png" height="32px"> Invidious
```  
APP_URL = https://inv.nadeko.net
APP_NAME = Invidious
VERSION = 1.0
CATEGORIES = AudioVideo;Player;Network;Video
ICON_SRC = https://raw.githubusercontent.com/iv-org/invidious/refs/heads/master/assets/android-chrome-512x512.png
```

#### <img src="https://cdn-icons-png.flaticon.com/512/1384/1384060.png" height="32px"> YouTube
```  
APP_URL = https://www.youtube.com
APP_NAME = YouTube
VERSION = 1.0
CATEGORIES = AudioVideo;Player;Network;Video
ICON_SRC = https://cdn-icons-png.flaticon.com/512/1384/1384060.png
```
