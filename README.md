# Part 1: Cryptocurrency Ticker Receiver Library in C++
* 環境安裝：
    * 本專案主要在 MacOS 的環境開發，若欲使用 MacOS 檢視本專案，請切換至 main 分支
        * Dependencies 安裝:
            * `brew install boost`
            * `brew install websocketpp`
    * 考量到環境問題，後續新增了能夠在 ubuntu 環境運行的分支，分支名稱：ubuntu
        * Dependencies 安裝:
            * `sudo apt install libssl-dev`
            * `sudo apt install libboost-all-dev`
            * `sudo apt install libwebsocketpp-dev`
    * 若在編譯過程中遇到錯誤，請根據提示安裝相應的函式庫，即可解決問題
    * 本專案已在一個展示用的開發機完成環境安裝和代碼部署，連上機器後可以直接檢視本專案：
        * 連線：`ssh ubuntu@3.25.245.168`
        * 密碼：`vici0000`
        * 註記：
            * 此展示用的開發機由於硬體設備受限，請勿在此環境進行編譯，機器會因此卡死
            * 連上機器後 `cd Assignment`，可直接執行 `./bin/ticker_app` 開始操作程式
* 原始碼請參考 `./src` 內的所有檔案
* 編譯方式可以採用整體重新編譯: `make clean; make` (須先安裝相關套件，請勿在展示用開發機進行編譯)
* `./bin` 包含預先編譯好的執行檔，執行方式 `./bin/ticker_app` (須切換至環境對應分支)
* 程式啟動以後會有詳細的操作說明，請依照說明操作 (啟動後輸入 `cheatsheet` 查看指令範例)

# Part 2: Explain how you would approach identifying performance bottlenecks in a library. Consider a library that you've recently worked with that exhibited slower performance than expected.
* 在這份作業中，我使用了 websocketpp 函式庫處理網路連線。以下是我選擇該函式庫的原因：
    * 使用 C++ 實作，具有良好的效能，且與本次作業相容。
    * 提供完善的文件說明。
    * 提供眾多使用範例供參考。
* 由於 websocketpp 是一個龐大的函式庫，無法進行全面的檢查。因此，我會從使用的角度來尋找可能的效能問題。
* 舉例來說，我的程式主要包含四種操作：1. 連線（connect）2. 中斷連線（disconnect）3. 訂閱（subscribe）4. 取消訂閱（unsubscribe）。我首先計算這四種操作的平均執行時間，結果如下：
    * 連線：0.6821 秒 / 1000 次
    * 中斷連線：0.4438 秒 / 1000 次
    * 訂閱：0.1642 秒 / 1000 次
    * 取消訂閱：0.1315 秒 / 1000 次
* 從這些結果中，我可以推斷連線和中斷連線操作的效能較差。我認為這可能是因為相較於簡單的資料傳輸，客戶端與伺服器端之間的連線建立和關閉涉及更多的互動步驟，因此較緩慢是合理的。
* 不過值得注意的是，由於這是一個網路程式，這種簡單的效能測試無法確定是 websocketpp 函式庫的問題，還是網路的問題，或伺服器的問題。因此，這樣的測試僅供開發者參考，以便在實際應用開發中進行優化。
* 在開發過程中，我運用多執行緒的概念來優化程式的執行。這是因為我需要同時處理使用者操作和監聽伺服器訊息。因此，我使用了兩個執行緒分別處理這兩項任務。