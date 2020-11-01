/*!
 * MindPlus
 * mpython
 *
 */
#include <MPython.h>
#include <DFRobot_Iot.h>
#include <DFRobot_DS18B20.h>
#include <mPython_tinywebdb.h>
#include <DFRobot_HuskyLens.h>

// 动态变量
String mind_s_XingMing;
// 静态常量
const String topics[5] = {"","","","",""};
// 创建对象
DFRobot_Iot       myIot;
mPython_TinyWebDB mydb;
DFRobot_HuskyLens huskylens;
DFRobot_DS18B20   ds18b20_P0;


// 主程序开始
void setup() {
	mPython.begin();
	ds18b20_P0.begin(P0);
	myIot.wifiConnect("abc", "11111111");
	while (!myIot.wifiStatus()) {yield();}
	display.setCursorLine(1);
	display.printLine("wifi连接成功");
	mydb.setServerParameter("http://tinywebdb.appinventor.space/api", "lossi","23dd7da7");
	myIot.init("iot.dfrobot.com.cn","","","",topics,1883);
	myIot.connect();
	while (!myIot.connected()) {yield();}
	display.setCursorLine(2);
	display.printLine("MQTT连接成功");
	huskylens.beginI2CUntilSuccess();
	huskylens.writeAlgorithm(ALGORITHM_FACE_RECOGNITION);
	delay(3000);
	display.fillScreen(0);
}
void loop() {
	huskylens.request();
	if (huskylens.isAppearDirect(HUSKYLENSResultBlock)) {
		if (huskylens.isLearned(huskylens.readBlockCenterParameterDirect().ID)) {
			mind_s_XingMing = mydb.getTag((String(huskylens.readBlockCenterParameterDirect().ID)));
			display.setCursorLine(1);
			display.printLine(mind_s_XingMing);
			display.setCursorLine(2);
			display.printLine("请保持面向摄像头");
			display.setCursorLine(3);
			display.printLine("3秒后开始测温");
			display.setCursorLine(4);
			display.printLine(ds18b20_P0.getTempC());
		}
	}
}
