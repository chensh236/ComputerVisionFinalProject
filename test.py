import tensorflow as tf 
from PIL import Image
import numpy as np
import matplotlib.pyplot as plt

#生成整数型的属性
def _int64_feature(value):
    return tf.train.Feature(int64_list = tf.train.Int64List(value = [value]))
 
#生成字符串类型的属性
def _bytes_feature(value):
    return tf.train.Feature(bytes_list = tf.train.BytesList(value = [value]))

def _create_file():
    writer= tf.python_io.TFRecordWriter("./A4Warping/hand/numbersGenerate.tfrecords") #要生成的文件
    for i in range(10):
        var = "./A4Warping/temp/img" + str(i) + "/guide.txt"
        reader = np.loadtxt(var, dtype = int)
        for j in range(len(reader)):
            col = reader[j]
            for k in range(col):
                fileStr = "./A4Warping/temp/img" + str(i) + "/" + str(j) + "_" + str(k) + ".bmp"
                img=Image.open(fileStr)
                img= img.resize((28,28))
#                plt.imshow(img)  #显示需要识别的图片
#                plt.show()
                img_raw=img.tobytes()
                example = tf.train.Example(features = tf.train.Features(feature = {
                        'label':_int64_feature(i),
                        'img_raw': _bytes_feature(img_raw)
                        }))
                writer.write(example.SerializeToString())
    writer.close()


    # Define the model (same as when creating the model file)
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

def detection():
    x = tf.placeholder(tf.float32, [None, 784])

    W_conv1 = weight_variable([5, 5, 1, 32], name = 'W1')
    b_conv1 = bias_variable([32],  name = 'B1')

    x_image = tf.reshape(x,[-1,28,28,1])

    h_conv1 = tf.nn.relu(conv2d(x_image,W_conv1) + b_conv1)
    h_pool1 = max_pool_2x2(h_conv1)

    W_conv2 = weight_variable([5, 5, 32, 64],  name = 'W2')
    b_conv2 = bias_variable([64],  name = 'B2')

    h_conv2 = tf.nn.relu(conv2d(h_pool1, W_conv2) + b_conv2)
    h_pool2 = max_pool_2x2(h_conv2)

    W_fc1 = weight_variable([7 * 7 * 64, 1024],  name = 'W3')
    b_fc1 = bias_variable([1024],  name = 'B3')

    h_pool2_flat = tf.reshape(h_pool2, [-1, 7*7*64])
    h_fc1 = tf.nn.relu(tf.matmul(h_pool2_flat, W_fc1) + b_fc1)

    keep_prob = tf.placeholder("float")
    h_fc1_drop = tf.nn.dropout(h_fc1, keep_prob)

    W_fc2 = weight_variable([1024, 10], name = 'W4')
    b_fc2 = bias_variable([10], name = 'B4')

    y_conv=tf.nn.softmax(tf.matmul(h_fc1_drop, W_fc2) + b_fc2)

    variables_dict = {'W1': W_conv1, 'B1': b_conv1, 'W2': W_conv2, 'B2': b_conv2, 'W3': W_fc1, 'B3': b_fc1, 'W4': W_fc2, 'B4': b_fc2}
    saver = tf.train.Saver(variables_dict)
    
    
    
    tfrecords_filename = "./A4Warping/hand/numbersGenerate.tfrecords"
    filename_queue = tf.train.string_input_producer([tfrecords_filename],) #读入流中
    reader = tf.TFRecordReader()
    _, serialized_example = reader.read(filename_queue)   #返回文件名和文件
    features = tf.parse_single_example(serialized_example,
                                       features={
                                           'label': tf.FixedLenFeature([], tf.int64),
                                           'img_raw' : tf.FixedLenFeature([], tf.string),
                                       })  #取出包含image和label的feature对象
    image = tf.decode_raw(features['img_raw'], tf.uint8)
    image = tf.reshape(image, [28, 28, 3])  #reshape为128*128的3通道图片
#    img = tf.cast(img, tf.float32) * (1. / 255) - 0.5 #在流中抛出img张量
    label = tf.cast(features['label'], tf.int32) #在流中抛出label张量
    with tf.Session() as sess: #开始一个会话
        sess.run(tf.global_variables_initializer())
        coord=tf.train.Coordinator()
        threads= tf.train.start_queue_runners(coord=coord)
        saver.restore(sess, "./A4Warping/hand/SAVE/model.ckpt") #使用模型，参数和之前的代码保持一致
        for i in range(10):
            writePath = "./A4Warping/temp/img" + str(i) + "/result.txt"
            with open(writePath, 'w') as f:
                dictionary= "./A4Warping/temp/img" + str(i) + "/guide.txt"
                reader = np.loadtxt(dictionary, dtype = int)
                for j in range(len(reader)):
                    col = reader[j]
                    for k in range(col):
                        example, l = sess.run([image,label])#在会话中取出image和label
                        img = Image.fromarray(example, 'RGB')#这里Image是之前提到的
                        img = img.convert('L')
#                        plt.imshow(img)  #显示需要识别的图片
#                        plt.show()
                        tv = list(img.getdata())
                        # normalization
                        tva = [(255-x)*1.0/255.0 for x in tv] 
                        prediction = tf.argmax(y_conv,1)
                        predint=prediction.eval(feed_dict={x: [tva],keep_prob: 1.0}, session=sess)
                        f.write(str(predint[0]))
#                        print(str(predint[0]))
                    f.write("\n")
                print("finish" + str(i))
        coord.request_stop()
        coord.join(threads)
 

if __name__ == '__main__':
    _create_file()
    print("create TFRecord finish!")
    detection()
#   for i in range(10):
#       writePath = "../temp/img" + str(i) + "/result.txt"
#       with open(writePath, 'w') as f:
#           var = "../temp/img" + str(i) + "/guide.txt"
#           reader = np.loadtxt(var, dtype = int)
#           for j in range(len(reader)):
#               col = reader[j]
#               for k in range(col):
#                   fileStr = "../temp/img" + str(i) + "/" + str(j) + "_" + str(k) + ".bmp"
#                   result = detection(fileStr)
#                   print(result)
#                   f.write(str(result))
#                   f.write(" ")
#               f.write("\n")
#           print("finish" + str(i))
#     for i in range(10):
#         for j in range(10):
#             fileStr = "./test/" + str(i) + "_" + str(1+ j) + ".png"
#             result = detection(fileStr)
#             print(result)
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                