# COP 4635 Project 1A

![Points badge](../../blob/badges/.github/badges/points.svg)

You want to send your hacker friends an encrypted message. Using the normal messengers doesn't seem like a good option to you, as you do not trust anybody with this highly sensitive data. Also, you don't want to let anybody know who the message received.

Therefore, you decide to set up your own webserver. Your friends should use a special link to access the webpage with the encrypted message. The message is encrypted using online tools, for example [this one](https://www.gillmeister-software.com/online-tools/text/encrypt-decrypt-text.aspx).

## The task

Create a C/C++ program using vanilla linux tcp networking system calls, which is called like this

    ./serveMessage <port> <encryptedMessage>

Accessing the webpage with the browser at `/message` should display an html webpage with the following content

```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8"/>
    <meta name="viewport" content="width=device-width, initial-scale=1.0"/>
    <title>Highly sensitive data</title>
</head>
<body>
    <p>encryptedMessage</p>
</body>
</html>
```

where `encryptedMessage` is replaced with the actual message. Any other link should return a 404 status code with `Not found` as message.

### Implementation details

The exact implementation is up to you. The only condition is that you use Linux socket programming. This means that the functions for networking should be `socket`, `bind`, `listen`, `accept`, `recv`, `send`, and so on. You are completely free on how to deal with the rest, e. g. using c or c++ strings, determining the address, etc.

- Do not change anything in the .github folder. Doing so will be considered attempted deception. If you change or delete it by accident, ask the teacher or one of the TAs for help.
- Have a look at the examples [examples/tcpClient3-1.c](examples/tcpClient3-1.c) and [examples/tcpServer3-1.c](examples/tcpServer3-1.c). Your implementation should follow these and use no additional libraries.
- Put your code into [src/main.cpp](src/main.cpp). It should run with the code in [run.bash](run.bash).

    ```console
    bash run.bash
    ```

    Test the webserver using your web browser at [http://localhost:8080/message](http://localhost:8080/message). If you wonder, the password to decrypt the message is `Cybernauts`.

- A typical GET request the Server will receive looks like this

    ```console
    GET / HTTP/1.1
    Host: 127.0.0.1:5678
    User-Agent: Go-http-client/1.1
    Accept-Encoding: gzip
    
    ```

- A typical GET response the Server should send is

    ```console
    HTTP/1.1 200 OK
    Content-Length: 217
    Content-Type: text/html; charset=utf-8
    
    <!DOCTYPE html><html lang="en"><head><meta charset="UTF-8"/><meta name="viewport" content="width=device-width, initial-scale=1.0"/><title>Highly sensitive data</title></head><body><p>encryptedMessage</p></body></html>
    ```

    or when the request url is invalid

    ```console
    HTTP/1.1 404 Object Not Found
    Content-Length: 210
    Content-Type: text/html; charset=utf-8
    
    <!DOCTYPE html><html lang="en"><head><meta charset="UTF-8"/><meta name="viewport" content="width=device-width, initial-scale=1.0"/><title>Highly sensitive data</title></head><body><p>Not found</p></body></html>
    ```

    Make sure that `Content-Length` is the actual length of the content. The content length is then exactly the length of the string `<!DOCTYPE html>...</body></html>` after the two newlines.

- The newlines in the GET requests and responses are important and should be done using `\r\n`.

- Do not send zero bytes over the internet connection, as this may result in strange behavior.

### Submission process

Your project is graded using Github. We do not accept any other submission method.

### Grading

You will get 90 points if the test cases succeed and your code is implemented as intended. The remaining 10 points will be given manually by the TA and judge your coding style. A good coding style means:

- Useful variable and function names
- Comments to explain what the code does
- Proper indentation

In total, this will add up to 100 points.
