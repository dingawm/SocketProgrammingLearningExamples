/*
 * server.c 为服务器端代码
*/

#include "config.h"

int 
main(int argc, char **argv)
{
	// 声明服务器地址和客户连接地址
	struct sockaddr_in servaddr, cliaddr;

	// 声明服务器监听套接字和客户端连接套接字
	int listenfd, connfd;
	pid_t childpid;

	// 声明缓冲区
	char buf[MAX_LINE];

	socklen_t clilen;

	// 1. 初始化监听套接字 listenfd
	if ( (listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
		perror("socket error");
		exit(1);
	} // if

	// 2. 设置服务器 sockaddr_in 结构体，即指定协议族、接受的 IP 地址、本地端口等
	bzero(&servaddr, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); // 表明可以接受任意 IP 地址
	servaddr.sin_port = htons(PORT);

	// 3. 绑定套接字和端口
	if ( bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0 ) {
		perror("bind error");
		exit(1);
	} // if

	// 4. 监听客户请求
	if ( listen(listenfd, LISTENQ) < 0 ) {
		perror("listen error");
		exit(1);
	} // if

	// 5. 接受客户请求
	for ( ; ; ) {
		clilen = sizeof(cliaddr);
		if ( (connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &clilen)) < 0 ) {
			perror("accept error");
			exit(1);
		} // if

		// 新建子进程单独处理连接
		if ( (childpid = fork()) == 0 ) {
			// child process
			close(listenfd);
			// str_echo
			ssize_t n;
			char buff[MAX_LINE];
			while ( (n = read(connfd, buff, MAX_LINE)) > 0 ) {
				write(connfd, buff, n);
			} // while
			exit(0);
		} // if
		close(connfd);
	} // for

	// 6. 关闭监听套接字
	close(listenfd);
}
