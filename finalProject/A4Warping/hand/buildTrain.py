import os
import tensorflow as tf
from PIL import Image  #注意Image,后面会用到
import matplotlib.pyplot as plt
import numpy as np
 
#生成整数型的属性
def _int64_feature(value):
    return tf.train.Feature(int64_list = tf.train.Int64List(value = [value]))
 
#生成字符串类型的属性
def _bytes_feature(value):
    return tf.train.Feature(bytes_list = tf.train.BytesList(value = [value]))
 
#制作TFRecord格式
def createTFRecord():
    data_dir = './trainSet/'
    classes={'0', '1', '2', '3', '4', '5', '6', '7', '8','9'}
    #输出TFRecord文件的地址
  
    writer = tf.python_io.TFRecordWriter("pic_train.tfrecords")
 
    for index,name in enumerate(classes):
        class_path=data_dir+ str(index) +'/'
        for img_name in os.listdir(class_path):
            img_path = class_path + img_name   #每个图片的地址
            img = Image.open(img_path)
            img= img.resize((28,28))
#            plt.imshow(img)  #显示需要识别的图片
#            plt.show()
#            print(index)
            img_raw = img.tobytes()          #将图片转化成二进制格式
            example = tf.train.Example(features = tf.train.Features(feature = {
                'label':_int64_feature(index),
                'img_raw': _bytes_feature(img_raw)
            }))
            writer.write(example.SerializeToString())
    writer.close()

if __name__ == '__main__':
    createTFRecord()