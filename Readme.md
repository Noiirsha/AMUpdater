# Custom AMUpdater

<img width="1366" height="768" alt="image" src="https://github.com/user-attachments/assets/316053b7-ac96-423c-b72d-c6a873f28946" />



>  *(Originally, this was just written for the MAXIMUM TUNE series to update the event files.)*
>
>  *This thing has absolutely nothing to do with the original AMUpdater and doesn’t actually communicate with AMAuthd (e.g., to make the server display the latest revision).*
>
>  *Haven’t written C++ in years, so forgive my  very shitty code.*
>
>  *btw I forcibly disabled SSL verification.*



## How to use?

Compile the file, link the libs, and place it into `AMCUS` folder, the program will read `AMConfig.ini` as its config file.

`cacfg-auth_server_url` will be used as start of actual request url

(e.g. `https://v388-front.mucha-prd.nbgi-amnet.jp:10082/mucha/front/`, the request will send to server like this

`https://v388-front.mucha-prd.nbgi-amnet.jp:10082/mucha/front/updater_poweron`) 

`dl_image`: 7z file without extension. the file will be extract to the upper folder(`../AMCUS` = `Game Folder`)
it's used by `GET /updater_getfile`, which can receive the update file from the server

the file directory of `sendFileToClient.7z (dl_image)` should like this:

```
the file structure of sendFileToClient.7z:
|- data_jp
	|- menu
		|-Zenichi
			|- ZenichiConfig.lua
|- data
	|- ...
```



## Requests from this updater

### 1. `GET /updater_poweron`

```json
{
	"server": "alive"
}
```

### 2. `GET /updater_getrevision`

```json
{
	"revision": "X.XX.XX"
}
```

### 3. `GET /updater_getfileinfo`

```json
"updateInfo": [
        {
            "path": "../data_jp/xxx/lua/",
            "fileName": "1.lua",
            "md5": "12345678901234567890123456789012"
        },
        {
            "path": "../data/xxx/lua/",
            "fileName": "2.lua",
            "md5": "12345678901234567890123456789012"
        }
    ]
```

### 4. `GET /updater_getfile`

```
Header:
	Content-Length: your_file_size

*res.sendFile()*
```

## *Change the include path in project setting with SDL3*

*lib used in the project*: `libcurl`, `7z`, `SDL3`



