# 建置的環境：
* ubuntu 18.04，g++版本為7開頭，瀏覽器成功的只有firefox，chrome匯入CA後沒有成功連線到。
# 實作功能：
* File copy
# Server：
* 將專案clone下來後再terminal輸入make後將程式碼編譯
* 輸入./server便能夠執行server，執行時需指定server的ip、CA及憑證的位置，以下為範例：
    * ./server IP 127.0.0.1 CA CA.crt SERVER_CRT server.crt SERVER_KEY server.key
    * port固定為8080
    
* 產生憑證的方式有兩種，第一次需要產生CA，第一次以後則不需要，兩種方法產生指令如下：
    * make genCrtFirst
    * make genCrt
* 輸入後就會產生憑證在Crt的資料夾內，壓所檔內已有產生好的憑證
# Client
* server啟動後client在瀏覽器上輸入server之ip與port(8080)後連上server
* 成果如下：
![](https://i.imgur.com/Ch2T03c.png)
連線後能夠查看server上的檔案，能夠自由的進出不同路徑
![](https://i.imgur.com/cjEXYEP.png)
![](https://i.imgur.com/VLGwoX8.png)
點選檔案後會彈出下載窗詢問client是否要下載
![](https://i.imgur.com/o78yynZ.png)

* 成果圖中我將ip對應到domain名稱b10615038，而沒有直接輸入ip，輸入ip也沒有問題