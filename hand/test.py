from PIL import Image, ImageFilter
import tensorflow as tf
import matplotlib.pyplot as plt
import numpy as np

def imageprepare(filePath):
    """
    This function returns the pixel values.
    The imput is a png file location.
    """
    #in terminal 'mogrify -format png *.jpg' convert jpg to png
    im = Image.open(filePath)
    # plt.imshow(im)
#    plt.show()
    im = im.convert('L')
    
    tv = list(im.getdata()) #get pixel values

    #normalize pixels to 0 and 1. 0 is pure white, 1 is pure black.
    tva = [ (255-x)*1.0/255.0 for x in tv] 
#    print(tva)
    return tva



    """
    This function returns the predicted integer.
    The imput is the pixel values from the imageprepare() function.
    """

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

def detection(imgList):

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
    
    resultArr = []
    for i in range(len(imgList)):
        img = imageprepare(imgList[i])
        with tf.Session() as sess:
            saver.restore(sess, "./SAVE/model.ckpt")#这里使用了之前保存的模型参数
            sess.run(tf.global_variables_initializer())
        #print ("Model restored.")
#        print('begin to predict')
            prediction=tf.argmax(y_conv,1)
            predint=prediction.eval(feed_dict={x: [img],keep_prob: 1.0}, session=sess)
#        print(h_conv2)
            resultArr.append(predint[0])
            print(predint[0])
    return resultArr

if __name__ == '__main__':
   for i in range(10):
       writePath = "../temp/img" + str(i) + "/result.txt"
       with open(writePath, 'w') as f:
           var = "../temp/img" + str(i) + "/guide.txt"
           reader = np.loadtxt(var, dtype = int)
           imgList = []
           for j in range(len(reader)):
               col = reader[j]
               for k in range(col):
                   fileStr = "../temp/img" + str(i) + "/" + str(j) + "_" + str(k) + ".bmp"
                   img = imageprepare(fileStr)
                   imgList.append(img)
           resultArr = detection(imgList)
           index = 0
           for j in range(len(reader)):
               col = reader[j]
               for k in range(col):          
                   fileStr = "../temp/img" + str(i) + "/" + str(j) + "_" + str(k) + ".bmp"
                   print(fileStr)
                   f.write(str(resultArr[index]))
                   f.write(" ")
                   index = index + 1
               f.write("\n")
           print("finish" + str(i))
    # imgList = []
    # for i in range(10):
    #     for j in range(10):
    #         fileStr = "./test/" + str(i) + "_" + str(1+ j) + ".png"
    #         imgList.append(fileStr)
    # resultArr = detection(imgList)
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                