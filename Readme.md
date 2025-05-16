# Custom AMUpdater

![image](https://github.com/user-attachments/assets/8c0d3c7f-65fd-4187-b8d0-652f7b240a4c)


>  *(Originally, this was just written for the MAXIMUM TUNE series to update the event files.)*
>
> *This thing has absolutely nothing to do with the original AMUpdater and doesn’t actually communicate with AMAuthd (e.g., to make the server display the latest revision).*
>
> *Haven’t written C++ in years, so forgive my  very shitty code.*
>
> *btw I forcibly disabled SSL verification.*

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
	Content-Length: 10240000

*res.sendFile()*
```

## *Change the include path in project setting with SDL3*
