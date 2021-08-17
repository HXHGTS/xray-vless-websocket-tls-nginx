#include <stdio.h>
#include <stdlib.h>


FILE* config,* cer;
char uuid[40],sni[30];
int mode;

int main(){
Menu:UI();
    system("clear");
    if (mode == 1) {
        install_xray();
        goto Menu;
    }
    else if (mode == 2) {
        system("systemctl stop xray");
        system("systemctl stop nginx");
        system("systemctl start xray");
        system("systemctl start nginx");
        printf("正在检测xray与nginx运行状态，以下输出不为空则运行正常！\n");
        printf("--------------以下输出不为空则xray运行正常------------------\n");
        system("ss -lp | grep xray");
        printf("\n--------------以下输出不为空则nginx运行正常------------------\n");
        system("ss -lp | grep nginx");
        printf("--------------------------------------------------------\n");
        goto Menu;
    }
    else if (mode == 3) {
        printf("xray二维码:\n\n");
        system("qrencode -t ansiutf8 < /usr/local/etc/xray/vless.txt");
        printf("\n\n");
        printf("VLESS链接:\n\n");
        system("cat /usr/local/etc/xray/vless.txt");
        printf("\n--------------------------------------------------------\n");
        printf("server: %s\nport: 2053\nUUID: %s\npath: /ISODownload\n",sni,uuid);
        printf("--------------------------------------------------------\n");
        goto Menu;
    }
    else if (mode == 4) {
        system("vi /usr/local/etc/xray/config.json");
        system("systemctl restart xray");
        printf("正在检测xray与nginx运行状态，以下输出不为空则运行正常！\n");
        printf("--------------以下输出不为空则xray运行正常------------------\n");
        system("ss -lp | grep xray");
        printf("\n--------------以下输出不为空则nginx运行正常------------------\n");
        system("ss -lp | grep nginx");
        printf("--------------------------------------------------------\n");
        goto Menu;
    }
    else if (mode == 5) {
        if (fopen("/root/1.pem", "r") == NULL || fopen("/root/2.pem", "r") == NULL) {
            printf("检测到证书与私钥文件未按照规定方式放置于根目录，强制退出！\n");
            exit(0);
        }
        printf("请输入已绑定此服务器ip的新域名:");
        scanf("%s", sni);
        config = fopen("/usr/local/etc/sni.conf", "w");
        fprintf(config, "%s", sni);
        fclose(config);
        config = fopen("/usr/local/etc/xray/uuid.conf", "r");
        fscanf(config, "%s", uuid);
        fclose(config);
        printf("正在复制SSL证书与私钥. . .\n");
        system("cp -rf /root/1.pem /usr/local/etc/xray/certificate.pem");
        system("cp -rf /root/2.pem /usr/local/etc/xray/private.pem");
        QRCodeGen();
        printf("正在检测xray与nginx运行状态，以下输出不为空则运行正常！\n");
        printf("--------------以下输出不为空则xray运行正常------------------\n");
        system("ss -lp | grep xray");
        printf("\n--------------以下输出不为空则nginx运行正常------------------\n");
        system("ss -lp | grep nginx");
        printf("--------------------------------------------------------\n");
        printf("xray部署完成！\n");
        printf("xray二维码:\n\n");
        system("qrencode -t ansiutf8 < /usr/local/etc/xray/vless.txt");
        printf("\n\n");
        printf("VLESS链接:\n\n");
        system("cat /usr/local/etc/xray/vless.txt");
        printf("\n--------------------------------------------------------\n");
        printf("server: %s\nport: 2053\nUUID: %s\npath: /ISODownload\n",sni,uuid);
        printf("--------------------------------------------------------\n");
        goto Menu;
    }
    else if (mode == 6) {
        printf("正在更新xray主程序. . .\n");
        system("systemctl stop xray");
        system("wget https://cdn.jsdelivr.net/gh/XTLS/Xray-install/install-release.sh -O install-release.sh");
        system("chmod +x install-release.sh && bash install-release.sh");
        system("systemctl start xray");
        system("rm -rf install-release.sh");
        printf("xray主程序更新完成！\n");
        printf("正在检测xray与nginx运行状态，以下输出不为空则运行正常！\n");
        printf("--------------以下输出不为空则xray运行正常------------------\n");
        system("ss -lp | grep xray");
        printf("\n--------------以下输出不为空则nginx运行正常------------------\n");
        system("ss -lp | grep nginx");
        printf("--------------------------------------------------------\n");
        goto Menu;
    }
    else if (mode == 7) {
        system("systemctl stop xray");
        system("systemctl stop nginx");
        goto Menu;
    }
    else {
        exit(0);
    }
    return 0;
}

int UI() {
    printf("-----------------------------------------------------------\n");
    printf("----------------------xray安装工具-----------------------\n");
    printf("-----------------------------------------------------------\n");
    printf("安装前或需要更新SSL证书，请将证书(*.cer/*.crt/*.pem)与私钥(*.key/*.pem)分别命名为1.pem与2.pem，上传至服务器/root目录\n");
    printf("-----------------------------------------------------------\n");
    printf("----------------------当前Kernel版本-----------------------\n");
    system("uname -sr");
    printf("-----------------------------------------------------------\n");
    printf("1.安装xray\n2.运行xray\n3.显示配置\n4.修改xray配置\n5.更新域名与SSL证书\n6.更新xray\n7.关闭xray\n0.退出\n");
    printf("-----------------------------------------------------------\n");
    printf("请输入:");
    scanf("%d", &mode);
    return 0;
}

int install_xray() {
    KernelUpdate(); 
    config = fopen("/usr/local/etc/sni.conf", "r");
    fscanf(config, "%s", sni);
    fclose(config);
    system("setenforce 0");
    system("yum install -y epel-release nginx bind-utils qrencode");
    printf("正在运行xray安装脚本. . .\n");
    system("wget https://cdn.jsdelivr.net/gh/XTLS/Xray-install/install-release.sh -O install-release.sh");
    system("chmod +x install-release.sh && bash install-release.sh");
    system("rm -rf install-release.sh");
    system("rm -rf TCPO.sh");
    printf("正在复制SSL证书与私钥. . .\n");
    system("cp -rf /root/1.pem /usr/local/etc/xray/certificate.pem");
    system("cp -rf /root/2.pem /usr/local/etc/xray/private.pem");
    printf("正在生成配置文件. . .\n");
    system("curl https://raw.githubusercontent.com/HXHGTS/xray-vless-websocket-tls-nginx/main/config.json.1 > /usr/local/etc/xray/config.json");
    printf("正在生成UUID. . .\n");
    system("xray uuid > /usr/local/etc/xray/uuid.conf");
    config = fopen("/usr/local/etc/xray/uuid.conf", "r");
    fscanf(config, "%s", uuid);
    fclose(config);
    config = fopen("/usr/local/etc/xray/config.json", "a");
    fprintf(config, "                        \"id\": \"%s\",\n", uuid);
    fclose(config);
    system("curl https://raw.githubusercontent.com/HXHGTS/xray-vless-websocket-tls-nginx/main/config.json.2 >> /usr/local/etc/xray/config.json");
    config = fopen("/usr/local/etc/xray/config.json", "a");
    fprintf(config, "                        \"id\": \"%s\",\n", uuid);
    fclose(config);
    system("curl https://raw.githubusercontent.com/HXHGTS/xray-vless-websocket-tls-nginx/main/config.json.3 >> /usr/local/etc/xray/config.json");
    printf("正在启动xray并将xray写入开机引导项. . .\n");
    system("systemctl enable xray");
    system("systemctl start xray");
    printf("正在启动nginx并将nginx写入开机引导项. . .\n");
    system("systemctl enable nginx");
    system("systemctl start nginx");
    QRCodeGen();
    printf("正在检测xray与nginx运行状态，以下输出不为空则运行正常！\n");
    printf("--------------以下输出不为空则xray运行正常------------------\n");
    system("ss -lp | grep xray");
    printf("\n--------------以下输出不为空则nginx运行正常------------------\n");
    system("ss -lp | grep nginx");
    printf("--------------------------------------------------------\n");
    printf("xray部署完成！\n");
    printf("xray二维码:\n\n");
    system("qrencode -t ansiutf8 < /usr/local/etc/xray/vless.txt");
    printf("\n\n");
    printf("VLESS链接:\n\n");
    system("cat /usr/local/etc/xray/vless.txt");
    printf("\n--------------------------------------------------------\n");
    printf("server: %s\nport: 2053\nUUID: %s\npath: /ISODownload\n",sni,uuid);
    printf("--------------------------------------------------------\n");
    return 0;
}

int QRCodeGen() {
    config = fopen("/usr/local/etc/xray/vless.txt", "w"); 
    fprintf(config, "vless://%s@%s:2053?security=tls#%s\n",uuid,sni,sni);
    fclose(config);
    return 0;
}

int KernelUpdate() {
    if ((fopen("KernelUpdate.sh", "r")) == NULL) {
        if (fopen("/root/1.pem", "r") == NULL || fopen("/root/2.pem", "r") == NULL) {
        printf("检测到证书与私钥文件未按照规定方式放置于根目录，强制退出！\n");
        exit(0);
    }
        printf("请输入已绑定此服务器ip的域名:");
        scanf("%s", sni);
        config = fopen("/usr/local/etc/sni.conf", "w");
        fprintf(config, "%s", sni);
        fclose(config);
        printf("正在升级新内核. . .\n");
        system("wget https://cdn.jsdelivr.net/gh/HXHGTS/TCPOptimization/KernelUpdate.sh -O KernelUpdate.sh");
        system("chmod +x KernelUpdate.sh");
        printf("正在升级，将自动触发重启以应用配置. . .\n");
        system("bash KernelUpdate.sh");
    }
    else {
        system("curl -sSL https://cdn.jsdelivr.net/gh/HXHGTS/TCPOptimization/TCPO.sh | sh");
    }
    return 0;
}

