from tensorflow.examples.tutorials.mnist import input_data
from PIL import Image, ImageFilter
import tensorflow as tf
import matplotlib.pyplot as plt
import numpy as np
import os

def read_and_decode(filename):
#创建一个reader来读取TFRecord文件中的样例
    reader = tf.TFRecordReader()
    #创建一个队列来维护输入文件列表
    filename_queue = tf.train.string_input_producer([filename], shuffle = False, num_epochs = 4000 * 100)
    #从文件中读出一个样例，也可以使用read_up_to一次读取多个样例
    _,serialized_example = reader.read(filename_queue)
#     print _,serialized_example
 
    #解析读入的一个样例，如果需要解析多个，可以用parse_example
    features = tf.parse_single_example(
    serialized_example,
    features = {'label':tf.FixedLenFeature([], tf.int64),
               'img_raw': tf.FixedLenFeature([], tf.string),})
    #将字符串解析成图像对应的像素数组
    image = tf.decode_raw(features['img_raw'], tf.uint8)
    image = tf.reshape(image, [28, 28, 3])  #reshape为128*128的3通道图片
    labels = tf.cast(features['label'], tf.int32)
    return image, labels

def createBatch(filename,batchsize, numberBatch):
    images,labels = read_and_decode(filename)
    # 乱序参数
    min_after_dequeue = 4200
    capacity = min_after_dequeue + numberBatch * batchsize
 
    image_batch, label_batch = tf.train.shuffle_batch([images, labels], 
                                                        batch_size=batchsize,
                                                        capacity=capacity, 
                                                        min_after_dequeue=min_after_dequeue
                                                        )
    label_batch = tf.one_hot(label_batch,depth= 10)
    return image_batch, label_batch


x = tf.placeholder(tf.float32, [None, 784])

y_ = tf.placeholder(tf.float32, [None, 10])

def weight_variable(shape, name):
    initial = tf.truncated_normal(shape,stddev = 0.1)
    return tf.Variable(initial, name)

def bias_variable(shape, name):
    initial = tf.constant(0.1,shape = shape)
    return tf.Variable(initial, name)

def conv2d(x,W):
    return tf.nn.conv2d(x, W, strides = [1,1,1,1], padding = 'SAME')

def max_pool_2x2(x):
    return tf.nn.max_pool(x, ksize=[1,2,2,1], strides=[1,2,2,1], padding='SAME')

W_conv1 = weight_variable([5, 5, 1, 32], name = "W1")
b_conv1 = bias_variable([32], name = "B1")

x_image = tf.reshape(x,[-1,28,28,1])

h_conv1 = tf.nn.relu(conv2d(x_image,W_conv1) + b_conv1)
h_pool1 = max_pool_2x2(h_conv1)

W_conv2 = weight_variable([5, 5, 32, 64], name = "W2")
b_conv2 = bias_variable([64], name = "B2")

h_conv2 = tf.nn.relu(conv2d(h_pool1, W_conv2) + b_conv2)
h_pool2 = max_pool_2x2(h_conv2)

W_fc1 = weight_variable([7 * 7 * 64, 1024], name="W3")
b_fc1 = bias_variable([1024], name = "B3")

h_pool2_flat = tf.reshape(h_pool2, [-1, 7*7*64])
h_fc1 = tf.nn.relu(tf.matmul(h_pool2_flat, W_fc1) + b_fc1)

keep_prob = tf.placeholder("float")
h_fc1_drop = tf.nn.dropout(h_fc1, keep_prob)

W_fc2 = weight_variable([1024, 10], name="W4")
b_fc2 = bias_variable([10],name="B4")

y_conv=tf.nn.softmax(tf.matmul(h_fc1_drop, W_fc2) + b_fc2)

cross_entropy = -tf.reduce_sum(y_*tf.log(y_conv))
train_step = tf.train.AdamOptimizer(1e-4).minimize(cross_entropy)
correct_prediction = tf.equal(tf.argmax(y_conv,1), tf.argmax(y_,1))
accuracy = tf.reduce_mean(tf.cast(correct_prediction, "float"))
variables_dict = {'W1': W_conv1, 'B1': b_conv1, 'W2': W_conv2, 'B2': b_conv2, 'W3': W_fc1, 'B3': b_fc1, 'W4': W_fc2, 'B4': b_fc2}
saver = tf.train.Saver(variables_dict)
   
#
#filename_queue = tf.train.string_input_producer([tfrecords_filename],) #读入流中
#reader = tf.TFRecordReader()
#_, serialized_example = reader.read(filename_queue)   #返回文件名和文件
#features = tf.parse_single_example(serialized_example,
#                                       features={
#                                           'label': tf.FixedLenFeature([], tf.int64),
#                                           'img_raw' : tf.FixedLenFeature([], tf.string),
#                                       })  #取出包含image和label的feature对象
#image = tf.decode_raw(features['img_raw'], tf.uint8)
#image = tf.reshape(image, [28, 28, 3])  #reshape为128*128的3通道图片
##    img = tf.cast(img, tf.float32) * (1. / 255) - 0.5 #在流中抛出img张量
#label = tf.cast(features['label'], tf.int32) #在流中抛出label张量
train_filename = "pic_train.tfrecords"

numberBatch = 4000
batchsize = 100


image_batch, label_batch = createBatch(filename = train_filename,batchsize = batchsize, numberBatch = numberBatch)
with tf.Session() as sess: #开始一个会话
    initop = tf.group(tf.global_variables_initializer(),tf.local_variables_initializer())
    sess.run(initop)
    coord=tf.train.Coordinator()
    threads= tf.train.start_queue_runners(coord=coord)
    for index in range(numberBatch):
        example, l =  sess.run([image_batch,label_batch])
        # print("min-step:" + str(index))
        for i in range(batchsize):
            img = Image.fromarray(example[i], 'RGB')#这里Image是之前提到的
            img = img.convert('L')
#            plt.imshow(img)  #显示需要识别的图片
#            plt.show()
            tv = list(img.getdata())
                        # normalization
            tva = [(255-x)*1.0/255.0 for x in tv]
#            print( l[i] )
#            inputArr = []
#            if l == 0 :
#                inputArr = np.array([1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0])
#            if l == 1 :
#                inputArr = np.array([0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0])
#            if l == 2 :
#                inputArr = np.array([0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0])
#            if l == 3 :
#                inputArr = np.array([0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0])
#            if l == 4 :
#                inputArr = np.array([0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0])
#            if l == 5 :
#                inputArr = np.array([0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0])
#            if l == 6 :
#                inputArr = np.array([0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0])
#            if l == 7 :
#                inputArr = np.array([0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0])
#            if l == 8 :
#                inputArr = np.array([0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0])
#            if l == 9 :
#                inputArr = np.array([0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0])
#            
#                
        train_step.run(feed_dict={x: [tva], y_: [ l[i] ], keep_prob: 0.5})
        if index % 100 == 0:
            train_accuracy = accuracy.eval(feed_dict={
                x: [tva], y_: [ l[i] ], keep_prob: 1.0})
            print('step %d, training accuracy %g' % (index, train_accuracy))
    coord.request_stop()
    coord.join(threads)
    saver.save(sess, './SAVE/model.ckpt')
    
    
    
#    
##     createTFRecord(train_filename,mapfile)
#    test_filename = "/home/wc/DataSet/traffic/testTFRecord/test.tfrecords"
##     createTFRecord(test_filename,mapfile)
##    image_batch, label_batch = createBatch(filename = train_filename,batchsize = 2)
#    
    
#    with tf.Session() as sess:
#        initop = tf.group(tf.global_variables_initializer(),tf.local_variables_initializer())
#        sess.run(initop)
#        coord = tf.train.Coordinator()
#        threads = tf.train.start_queue_runners(sess = sess, coord = coord)
# 
#        try:
#            step = 0
#            while 1:
#                
#                step += 1
##                print step
#                print (_label_batch)
#        except tf.errors.OutOfRangeError:
#            print (" trainData done!")
