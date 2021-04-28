# GCC
`` yum install gcc cmake gcc-c++ gdb ``  

# VS Code Native Debug
debug gcc remotely, 可选中cpp文件进行debug, 根据launch.json中的设置

# Samba 配置
## 启动
```
yum install samba
systemctl stop firewalld
systemctl restart smb
```
确保setlinux关闭，可以用``setenforce 0``命令执行

## 扩展 link
```
[global]
follow symlinks = yes
unix extensions = no
wide links = yes

[share]
follow symlinks = yes
wide links = yes
```