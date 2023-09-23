# Crypto Ticker Receiver

I'm excited to share my Crypto Ticker Receiver Library with everyone. The design of this Ticker Receiver Library is aimed at supporting various exchanges, making it highly flexible and scalable for future expansion or adjustments to exchange APIs. Feel free to reference my project and use it as a foundation for developing your own applications. If you find this project helpful, please consider giving it a star.

## How to Use?

### Environment Setup:

- This project was primarily developed on macOS. If you intend to use macOS to view this project, please switch to the main branch.
    - Dependency Installation:
        - `brew install boost`
        - `brew install websocketpp`

- Considering environmental differences, an "ubuntu" branch has been added to support running in the Ubuntu environment.
    - Dependency Installation:
        - `sudo apt install libssl-dev`
        - `sudo apt install libboost-all-dev`
        - `sudo apt install libwebsocketpp-dev`

- If you encounter any errors during the compilation process, please follow the prompts to install the corresponding libraries to resolve the issue.

### Source Code:

- Please refer to all files in `./src` for the source code.

### Compilation:

- You can compile the code by running the following command:
    ```
    make clean; make
    ```

### Precompiled Executables:

- Precompiled executable files are available in `./bin`. To run them, use the following command (switch to the appropriate environment branch):
    ```
    ./bin/ticker_app
    ```

- Once the program is launched, you will receive detailed instructions on how to use it. Please follow the instructions. You can also enter `cheatsheet` after starting to view command examples.
