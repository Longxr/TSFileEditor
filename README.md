# 翻译文件（.ts）和Excel文件的相互转换工具

## 主要功能
### 生成待翻译字符串的excel文件
![生成excel文件](https://upload-images.jianshu.io/upload_images/2756183-05f5dfca13142009.gif?imageMogr2/auto-orient/strip)

.ts文件中的源文本作为excel的第一列key值，翻译文本(如果有的话)在抽出的时候放在第三列，没有则为空
![excel截图.png](https://upload-images.jianshu.io/upload_images/2756183-3ffd1670eb98846b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

因为我加入了一列key值，一般情况生成的话key就是你原来的源文本，如果要使用自动翻译功能，将第一列拷贝一份到第二列即可；也可以像我这样代码里代码里不写实际的文本，另外生成一个中文的翻译文件(ts)，将translation放上中文，需要其他语言的翻译的时候将中文拷贝到第二列即可

### 更新翻译文件(.ts)
* 如果有专门负责翻译的人的话，可以将excel第二列放源文本后给别人，让他把翻译好的字符串放在第三列。翻译好后，选择好excel文件和ts文件，点击*译文写入*按钮就可以愉快地导入翻译内容了
* 部分包含标点符号的地方可能有点小问题，可以打开Qt语言家检查下

### 自动翻译
* 将要翻译的excel文件第二列放要翻译的文本（重要！！！）
* 自动翻译功能调用的[有道智云的API](http://ai.youdao.com/index.s)，需要使用的话自己去注册一个账号，免费送100元体验金。注册好后添加自己的翻译实例和应用，并将你的应用ID和应用密钥填到程序代码的:`TSFileEditor\Source\MainApps\TSFileEditor\TranslateWorker.cpp`上边
![有道智云](https://upload-images.jianshu.io/upload_images/2756183-84c37cdd03fc8571.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![填写位置](https://upload-images.jianshu.io/upload_images/2756183-9617ef61d247349f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

以后有时间我再加个在主界面填写这个ID和密钥的地方(￣▽￣)"（懒得改....）

* 下面是中文翻译成日文的栗子：
![translate.gif](https://upload-images.jianshu.io/upload_images/2756183-e3f5cdb8e80f10ea.gif?imageMogr2/auto-orient/strip)

点击*excel生成*按钮后，就可以得到翻译好的excel了：
![翻译结果](https://upload-images.jianshu.io/upload_images/2756183-229843327e932b73.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

* 然后可以结合功能二来更新翻译文件(ts)，之后就可以愉快地发布出(qm)给程序用了
