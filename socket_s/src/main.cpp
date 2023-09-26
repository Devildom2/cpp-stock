#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "Logger.h"

int main() {

    int sockfd;
    // ソケット生成
    if( ( sockfd = socket( AF_INET, SOCK_STREAM, 0 ) ) < 0 ) {
        perror( "socket" );
    }

    struct sockaddr_in addr;
    // 待ち受け用IP・ポート番号設定
    addr.sin_family = AF_INET;
    addr.sin_port = htons( 1234 );
    addr.sin_addr.s_addr = INADDR_ANY;

    socklen_t addr_len = sizeof( addr );

    // バインド
    if( bind( sockfd, (struct sockaddr *)&addr, addr_len ) < 0 ) {
        perror( "bind" );
    }

    // 受信待ち
    if( listen( sockfd, SOMAXCONN ) < 0 ) {
        perror( "listen" );
    }

    // クライアントからのコネクト要求待ち
    int client_sockfd;
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof( client_addr );

    if( ( client_sockfd = accept( sockfd, (struct sockaddr *)&client_addr, &client_addr_len ) ) < 0 ) {
        perror( "accept" );
    }
    
    char buf[1024];
    // 受信バッファ初期化
    memset( buf, 0, sizeof( buf ) );
 
    // 受信
    int rsize;
    while( 1 ) {
        rsize = recv( client_sockfd, buf, sizeof( buf ), 0 );
 
        if ( rsize == 0 ) {
            break;
        } else if ( rsize == -1 ) {
            perror( "recv" );
        } else {
            printf( "receive:%s\n", buf );
            sleep( 1 );
 
            // 応答
            printf( "send:%s\n", buf );
            write( client_sockfd, buf, rsize );
        }
    }
 
    // ソケットクローズ
    close( client_sockfd );
    close( sockfd );
 
    return 0;
}