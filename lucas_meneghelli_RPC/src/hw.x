struct param {
	int arg1;
	int arg2;
};

struct msg {
	char nickname[100];
	char content[1050000];
};

program PROG {
	version VERS {
		string func0(void)   		= 1;
		int    func1(string) 		= 2;
		int    func2(int)    		= 3;
		int    func3(param)  		= 4;
		int    sendchat(msg) 		= 5;
		int    getmsgindex(void)	= 6;
		msg    receivechat(int)		= 7;
		int    connect(string)		= 8;
	} = 1;
} = 0x30009999;
