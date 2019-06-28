#include "../src/xport.h"
#include <string>

using namespace xport;
using namespace std;

/**
 * 这个demo适合入门
 * 
 * 主要3个步骤
 * 1. 定义你的媒体类型。这里是FileMedia，来自本地视频文件
 * 2. 注册媒体类型。这里是FileMediaCreator
 * 3. 启动http服务
 */

//FileMedia 本地文件媒体。所有函数都可以简单地用FILE接口实现
class FileMedia : public SeekableMedia{
	FILE* mFp;
	int64_t mSize;
public:
	FileMedia(string path) {
		mFp = fopen(path.c_str(), "rb");
	}
public:
	bool open(){
		if (mFp){
			fseek(mFp, 0, SEEK_END);
			mSize = ftell(mFp);
			fseek(mFp, 0, SEEK_SET);
			printf("open success with size=%d\n", mSize);
			return true;
		}

		return false;
	}

    void close(){
		if (mFp)
			fclose(mFp);
	}

    std::string read(){
		char buf[4096]={0};
		auto ret = fread(buf, 1, sizeof(buf), mFp);

		if (ret <= 0)
			return "";
		return string(buf, ret);
	}

    int64_t seek(int64_t offset){
		return fseek(mFp, offset, SEEK_SET);
	}

    int64_t size(){
		return mSize;
	}

};

//FileMediaCreator 定义对于什么请求，返回FileMedia
class FileMediaCreator : public IMediaCreator{
public:
	int scoreRequest(MediaRequest& req){
		return 100;//作为一个简单的Demo，我们只有一种媒体类型，所以任何请求都创建FileMedia
	}

    IMedia* create(MediaRequest& req){
		//http://127.0.0.1:8000/media?path=/tmp/1.mp4
		auto path = req.getParamValue("path"); //获取请求的视频文件路径
		return new FileMedia(path);
	}
};

int main(int argc, char* argv[]){
	releaseOwnershipToRegsiter(new FileMediaCreator);

	//启动http服务
	HttpServer server;
	if (!server.start("127.0.0.1", 8000)){
		printf("start http server failed\n");
		return -1;
	}

	return 0;
}