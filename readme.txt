FTP服务器项目

功能概述：
	客户端可以通过输入以下命令进行服务器上的文件查看：
	1.cd 进入对应目录
	2.ls 列出相应目录文件
	3.puts 文件名 将本地文件上传至服务器
	4.gets 文件名 下载服务器文件到本地
	5.remove 文件名 删除服务器上文件
	6.pwd 显示目前所在目录
	7.其他命令不响应
额外功能：
	（现版本无gets功能，但原理和puts相同，up主有空再更）
	1.断点续传
	2.mmap大文件映射
项目启动方法：
	服务器：
	./server
	客户端：
	./client ip port





