# Cpp_Server_on_Linux
因為最近打算重K C++ 以及邊看邊實踐 《Effective C++》的內容， 這項專案主要是想練習如何透過C++來撰寫Server端的程式碼，並且在Linux上建立一個簡單的Server(伺服器主劉作業系統)。

### 主要練習項目
    * 熟悉 C++ 與 morden C++的實踐 
    * Linux 環境下的Server端程式撰寫
    * 熟悉TCP/IP等網路協議及socket的基本概念
    * 了解相關Linux os 的基本概念

### 環境
    * OS: Ubuntu 22.04.4 LTS
    * Compiler: g++ 7.5.0
    * IDE: Visual Studio Code
    * C++: C++17

### To Do
    * 高並發處理
    * 定時器
    * 日誌系統
    * HTTP協議解析
    * webbench測試
    * 文件下載 (可以續傳)
    * 靜態資源處理

### update

    4/18
    - 練習了linux socket 的使用，及建立一個會echo message的server
    - 了解了socket的desciption file的概念，及字元流的概念
    - 複習TCP/IP的基本概念
    
    4/19
    - study 伺服器模型
    - study I/O處理模型(blocking, nonblocking, multiplexing, signal driven I/O, asynchronous I/O)
    - study multiplexing I/O (select, poll, epoll)
    - study epoll model, Edge Triggered and Level Triggered
    - 實作epoll模型

    4/21
    - 重構socket成一個class
    - 重構epoll成一個class

    4/24
    - study reactor and proactor
    - study event driven model
    - 開始實作reactor模型，先封裝了 EventLoop 類

    4/25
    - 封裝了新的類 Event 類，用來封裝epoll_event的事件類型，因為覺得每個事件驅動的事件函數應該是與事件本身相關的，所以將事件函數封裝在Event類中

    4/29
    - 封裝了accpet事件的accpetor類,將各個部件都進行了封裝，解偶了各個模組並增加了可讀性與擴充性
    - 目前的reactor模型是單線程的，之後會進行多線程的改進  

    4/30
    - 封裝了tcp connection類，用來處理tcp的連接，並且將tcp connection類與event loop類進行了解耦
    - bind函數在填入參數時要小心，不要重複填入，否則有可能會銷毀原本的物件
    - 單線程的reactor模型已經完成，之後會進行多線程的改進