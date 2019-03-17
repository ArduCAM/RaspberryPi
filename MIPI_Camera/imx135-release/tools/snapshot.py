import cv2
import time
import v4l2_ctl

global frameCount,totalFrame,begin
global current_exposure,current_red_balance,current_blue_balance
def set_current_para():
    global current_exposure,current_red_balance,current_blue_balance
    #v4l2_ctl.write_blue_gain(current_blue_balance)
    #v4l2_ctl.write_red_gain(current_red_balance)
    v4l2_ctl.write_exposure(current_exposure + 1)
    print current_exposure#,current_red_balance,current_blue_balance
    
def take_picture(cap):
    global current_exposure,current_red_balance,current_blue_balance
    global totalFrame
    cap.read()
    set_current_para()
    for i in range(5) :
        cap.grab()
        #ret,frame = cap.read()
        #frame = cv2.resize(frame,(1280,960))
        #cv2.imshow("Test",frame)
        #cv2.waitKey(10)
    ret,frame = cap.read()
    cv2.imwrite("snapshot.jpg",frame)
    #displayImage(cap)
    '''
    while True:
        totalFrame += 1
        ret,frame = cap.read()
        cv2.imshow("Test",frame)
        cv2.waitKey(10)
        print "total frame = ",totalFrame
        #if totalFrame == 50:
        #    set_current_para()
    '''
    if 1280 < frame.shape[1]:
        scale = 1280.0 / frame.shape[1]
        print "scale %f,%f,%f"%(scale,frame.shape[0] * scale,frame.shape[1]*scale)
        frame = cv2.resize(frame,(int(frame.shape[1] * scale),int(frame.shape[0] * scale)))
    cv2.imshow("Test",frame)
    cv2.waitKey(5000)

def white_balance(KR,KB):
    global current_red_balance,current_blue_balance
    print "white balance"
    red_balance = v4l2_ctl.read_red_gain()
    blue_balance = v4l2_ctl.read_blue_gain()
    red_balance *= KR
    blue_balance *= KB
    v4l2_ctl.write_red_gain(red_balance)
    v4l2_ctl.write_blue_gain(blue_balance)
    current_red_balance=red_balance
    current_blue_balance=blue_balance
    pass

def auto_exposure(average):
    global current_exposure
    exposure_max = 4000
    exposure_min = 100
    brightness_min = 45
    brightness_max = 65
    print "auto exposure",average
    exposure_val = v4l2_ctl.read_exposure()
    current_exposure = exposure_val
    #print "exposure value = ",exposure_val
    if(average < brightness_min):
        exposure_val *= (brightness_min / average)
    elif average > brightness_max:
        exposure_val *=(brightness_max / average)
    else:
        return
    #print "write exposure = ",exposure_val
    if(exposure_val < exposure_min):
        exposure_val = exposure_min
        print "exposure value already minimum"
    elif exposure_val > exposure_max:
        exposure_val = exposure_max
        print "exposure value already maximum"
    v4l2_ctl.write_exposure(exposure_val)
    current_exposure=exposure_val
    #print "global exposure",current_exposure
    #pass


def displayImage(cap):
    global frameCount, totalFrame, begin
    while totalFrame < 20:
        rev, frame = cap.read()
        totalFrame += 1
        frameCount += 1
        #if(totalFrame == 1):
        #    v4l2_ctl.write_global_gain(2003)
        if(totalFrame % 5 == 0):
            #cv2.imwrite("Test%d.jpg"%totalFrame,frame)
            '''
            img_rgb = cv2.split(frame)
            B = cv2.mean(img_rgb[0])[0]
            G = cv2.mean(img_rgb[1])[0]
            R = cv2.mean(img_rgb[2])[0]
            KB = G/B
            KR = G/R
            print cv2.mean(frame)
            white_balance(KR, KB)
            '''
            auto_exposure(sum(cv2.mean(frame))/3.0)
        frame = cv2.resize(frame,(frame.shape[1] / 2,frame.shape[0]/2))
        cv2.imshow("Test", frame)
        cv2.moveWindow("Test",0,0)
        cv2.waitKey(10)
        if time.time() - begin >= 1:
            print "fps ", frameCount
            #img_rgb = cv2.split(frame)
            #print cv2.mean(img_rgb[0])[0], cv2.mean(img_rgb[1])[0], cv2.mean(img_rgb[2])[0]
            #print sum(cv2.mean(frame))/3.0
            frameCount = 0
            begin = time.time()
            #break

if __name__ == "__main__":
    totalTime = time.time()
    cap = cv2.VideoCapture(0)
    cap.set(cv2.cv.CV_CAP_PROP_FRAME_HEIGHT, 720)
    cap.set(cv2.cv.CV_CAP_PROP_FRAME_WIDTH, 1280)
    # cap.set(cv2.cv.CV_CAP_PROP_FPS, 60) 
    begin=time.time()
    totalFrame = 0
    frameCount = 0
    displayImage(cap)
    print "total frame ",totalFrame
    print "total time ",(time.time() - totalTime)
    totalFrame = 0
    cap.set(cv2.cv.CV_CAP_PROP_FRAME_HEIGHT, 3120)
    cap.set(cv2.cv.CV_CAP_PROP_FRAME_WIDTH,4192)
    #displayImage(cap)
    take_picture(cap)
    print "End"
    print "Total time = ",(time.time() - totalTime)
    '''
    rev,frame = cap.read()
    cv2.imshow("Test",frame)
    cv2.waitKey(-1)
    '''
    cv2.destroyAllWindows()
