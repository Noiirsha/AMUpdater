# 自定义AMUpdater

![image](https://github.com/user-attachments/assets/8c0d3c7f-65fd-4187-b8d0-652f7b240a4c)


>  (本意只是给湾岸写的 用来更新下活动信息文件)
>
> 这东西跟原版AMUpdater半毛钱关系也没有 也不会实际跟AMAuthd进行通信（比如让服务器显示最新修订版）
>
> 原版用的dx9实现的 我用的SDL3
>
> n年没写C++了 原谅我的史山代码
>
> 以及你懂的 我强制禁用了SSL检查

## 使用方法

连库带包丢进去AMCUS文件夹，这个AMUpdater会自动读取AMConfig来确定以下信息

```text
cacfg-auth_server_url       更新服务器URL         
cacfg-game_cd               游戏版本信息          
amucfg-title                游戏名                
amucfg-game_rev             大版本号			  
cacfg-game_ver              小版本号			  
amucfg-countdown            关闭倒计时			  
amdcfg-writableConfig       WritableConfig路径 	  
dtcfg-dl_image_path         镜像下载地址		
```

## 请求连接说明

1. ### `GET /updater_poweron`

 获取服务器存活情况

```json
{
	"server": "alive"
}
```

### 2. `GET /updater_getrevision`

获取版本号

```json
{
	"revision": "X.XX.XX"
}
```

### 3. `GET /updater_getfileinfo`

获取需要校验的文件

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

获取dl_image (格式为7z)

```
Header:
	Content-Length: 10240000

*res.sendFile()*
```

