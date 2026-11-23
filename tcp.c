/*
 * tcp_client.c —— TCP 发送程序（客户端）
 *
 * 编译: gcc tcp_client.c -o tcp_client
 * 运行: ./tcp_client <服务器IP> <端口> <要发送的消息>
 * 示例: ./tcp_client 127.0.0.1 8888 "hello tcp"
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    if (argc < 3) {
        fprintf(stderr, "用法: %s <服务器IP> <端口> [消息]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *server_ip = argv[1];
    int port = atoi(argv[2]);
    const char *msg = (argc >= 4) ? argv[3] : "hello tcp";

    /* 1. 创建 socket: AF_INET(IPv4) + SOCK_STREAM(TCP) */
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket 创建失败");
        exit(EXIT_FAILURE);
    }

    /* 2. 填写服务器地址 */
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
        fprintf(stderr, "无效的 IP 地址: %s\n", server_ip);
        close(sock);
        exit(EXIT_FAILURE);
    }

    /* 3. 建立连接 */
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("连接服务器失败");
        close(sock);
        exit(EXIT_FAILURE);
    }
    printf("[客户端] 已连接 %s:%d\n", server_ip, port);

    /* 4. 发送数据 */
    ssize_t n = send(sock, msg, strlen(msg), 0);
    if (n < 0) {
        perror("发送失败");
    } else {
        printf("[客户端] 已发送 %zd 字节: %s\n", n, msg);
    }

    /* 5. 关闭连接 */
    close(sock);
    return 0;
}

