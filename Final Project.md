# Final Project2

16340028 陈思航

## 算法的修改

### 自适应阈值法

在上一部分使用了 分块进行OSTU获得阈值的方法。但是这种方法很容易将数字切分成两个不同的块，从而导致数字形状的改变或者消失。在这一部分，为了应付100张不同的图，使用了自适应阈值的方法。自适应阈值方法能够在光线照射不均匀的情况下对图片二值化操作而效果与光照均匀时类似。

自适应阈值法过程如下：

- 求出图像的积分，即对图像按行进行遍历，再按列进行遍历，求出整张图像的积分值。
- 划出适当大小的邻域。通过之前的积分图像求得在邻域内求出该邻域像素平均值。
- 对该平均值设置一个权重。当这一点低于平均值与权重值的积的时候，设为0，否则设为255。

该方法相当于对图像进行一定半径的平滑之后再求出适当的二值化阈值，效果很好。

### 霍夫变换的修改

在之前直线检测的时候，投票阈值为一个固定的值。在这种情况下如果光线改变可能出现投票值过高或者过低的问题。所以投票阈值应该是可以变动的。在实验中设定投票阈值为Hough空间的最大值和一个权重的积。而且对于求得的alpha和p，不要转换位k和b，否则alpha等于90°的时候k不存在，b也不存在，会出现该直线消失的问题。所以通过直接保留alpha和p再通过这两个值求出直线的交点。对于可能出现的误差问题，只要该点的马尔科夫邻域内存在有符合另一直线方程的点，视为交点。

### 边缘的DFS搜索

对于二值化的图片，需要对其边缘通过DFS搜索 去掉因为原来纸张变形导致的Warping误差。这一部分误差二值化后为黑色，对内容的旋转造成很大的影响，所以通过DFS算法遍历得到所有的边缘，并去掉。具体方法可以参考图像分割中的DFS算法寻找需要识别的纸张，因为非常类似所以不赘述了。

## 实验结果

老师只给了92张图（10+82），其中有一张还不能用，所以一共有91张老师提供的图片。这些图片还不够刺激，所以我在复杂光线条件下拍了9张图片（较暗、开很大的闪光灯、有阴影、变形）等等。再对纸张进行Warping求得数字。

#### 分割

因为采用了新的算法， 很大程度上避免了噪点，所以不在灰度图上真实分割而是直接将自适应阈值法得到的图像进行分割。而对于旋转，因为得到的数字更小，所以直线更细，旋转角度也更加准确。在以上这些6因素的作用下，无论是之前的10张图片还是后面老师所给的图片，都有挺好的效果。对于最后10张图片，如果闪光灯很亮可能导致K-means后得到的纸张编程椭圆，造成直线检测的困难，所以K-means前高斯模糊的参数需要正确设置。

认为比较困难的分割(代表性)：

<table>
     <tr>
        <td><img src="finalProject/A4Warping/data/cut_71.bmp"></td>
        <td><img src="finalProject/A4Warping/data/cut_89.bmp"></td>
        <td><img src="finalProject/A4Warping/data/cut_94.bmp"></td>
        <td><img src="finalProject/A4Warping/data/cut_54.bmp"></td>
         <td><img src="finalProject/A4Warping/data/cut_18.bmp"></td>
    </tr>
    <tr>
        <td><img src="finalProject/A4Warping/data/71.bmp"></td>
        <td><img src="finalProject/A4Warping/data/89.bmp"></td>
         <td><img src="finalProject/A4Warping/data/94.bmp"></td>
        <td><img src="finalProject/A4Warping/data/54.bmp"></td>
                 <td><img src="finalProject/A4Warping/data/18.bmp"></td>
    </tr>
</table>



#### 识别

因为使用了比较贴近测试用的训练集，识别的准确率也是比较高的，特别是前面所给的90张图。对于最后几张图，因为分割的一些小问题会造成识别的错误（加了一两个数字进去）。对于CNN卷积神经网络的训练，每张图片的一半用于训练，再加上之前的训练集一共是5000张图片（3000手动分类+2000数据集图片）。至于MNIST，真的效果一般，因为不符合手写的习惯，为了得高分还是忍忍自己手动分类啦。训练步数4000，每个batch为100，训练时长20分钟。

通过进行对比，可以发现前10张图因为内容比较多对图片内容旋转的角度会造成一定的影响，也就影响了分割。而后面的80+张图因为只有三行所以内容的旋转比较简单，旋转角度也比较准确。所以其中的数字分割准确性大于前面的图片。而对于最后的几张图片，因为复杂光线导致自适应阈值法也不能很好分割，依旧产生了噪点。

![1545917721470](C:\Users\Chen Sihang\AppData\Roaming\Typora\typora-user-images\1545917721470.png)

![1545917440146](C:\Users\Chen Sihang\AppData\Roaming\Typora\typora-user-images\1545917440146.png)

总的来说，识别率是非常高的。其中有的同学没有按照顺序写，最后的几张图片第三行写的是QQ号码...。

这次实验的结果依旧存放在如下目录，因为太多我真的贴不了图了。

分割效果`\A4Warping\data\`

字符分割保存路径`\A4Warping\temp\`

识别结果`\A4Warping\result\result.csv`

CNN模型训练与数据集`\A4Warping\hand\`

一键运行`\start.bat`

做了好久而且花费了好多的精力，希望TA能给个高分，谢谢！

## 实验环境

```
CPU: R7-2700X
GPU: GTX1080
OS: Windows10 Pro
Python: 3.5
Tensorflow: 1.1.0
cuDNN:*
CUDA:V8.0
```

