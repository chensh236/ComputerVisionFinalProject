import tensorflow as tf 
from PIL import Image  #注意Image,后面会用到 
import numpy as np
import matplotlib.pyplot as plt
writer= tf.python_io.TFRecordWriter("numbersGenerate.tfrecords") #要生成的文件
for i in range(10):
    var = "../temp/img" + str(i) + "/guide.txt"
    reader = np.loadtxt(var, dtype = int)
    for j in range(len(reader)):
        col = reader[j]
        for k in range(col):
            fileStr = "../temp/img" + str(i) + "/" + str(j) + "_" + str(k) + ".bmp"
            img=Image.open(fileStr)
            img= img.resize((28,28))
#            plt.imshow(img)  #显示需要识别的图片
#            plt.show()
            img_raw=img.tobytes()
            example = tf.train.Example(features=tf.train.Features(feature={
            "label": tf.train.Feature(int64_list=tf.train.Int64List(value=[j])),
            'img_raw': tf.train.Feature(bytes_list=tf.train.BytesList(value=[img_raw]))
        })) #example对象对label和image数据进行封装
        writer.write(example.SerializeToString())  #序列化为字符串
writer.close()