''' intel D05 - 1280x720 ; 7 cm to 50cm= f .1mm obj detection at 7 cm; 90 fps

>>>>>>>>>>>>>>>>>>>>   Detect pensil object and give distance from camera >>>>>>>>>>>>>>>>>>>>>>>>>>
'''

import pyrealsense2 as rs  
import cv2
import imutils
import numpy as np
# create a pipeline
pipeline=rs.pipeline()

# configure the pipeline
config=rs.config()
w=640;h=480
config.enable_stream(rs.stream.color,w,h,rs.format.bgr8,30)
config.enable_stream(rs.stream.depth,w,h,rs.format.z16,30)

# start the pipeline
pipeline.start(config) # To start streaming

while True:
	# fetching color and depth information
	frames=pipeline.wait_for_frames()
	color_frame=frames.get_color_frame()  # rgb image frame
	depth_frame=frames.get_depth_frame()  # depth channel frame
	print('Depth frame info ',depth_frame)
	# convert depth data to a numpy array
	color_image=np.asanyarray(color_frame.get_data())
	depth_image= np.asanyarray(depth_frame.get_data())
	
	#print(f'depth_image_data :: {depth_image}')
	depth_cm=cv2.applyColorMap(cv2.convertScaleAbs(depth_image,alpha=0.2), cv2.COLORMAP_JET)
	# alphs=0.5 two different depth ;.1=10
	
	# show frames using cv2
	cv2.imshow('RGB', color_image)
	cv2.imshow('Depth',depth_image)
	cv2.imshow('Depth color map',depth_cm)


	### IMAGE PROCESSING and FIND Contour	
	hsv = cv2.cvtColor(color_image, cv2.COLOR_BGR2HSV)
	lower_green=np.array([60,20,20])
	upper_green=np.array([110,255,255])
	mask=cv2.inRange(hsv,lower_green,upper_green)   # 1 or 0 in range()
	res=cv2.bitwise_and(color_image,color_image,mask=mask)      # frame,where in the frame and mask is mask=1
	# blur mask
	#mask=cv2.GaussianBlur(mask,(11,11),cv2.BORDER_DEFAULT)
	#creat a circle over the detected object & find its center
	print("img_processed!")
	contours=cv2.findContours(mask,cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_NONE)
	contours=imutils.grab_contours(contours)

	for c in contours:
		area=cv2.contourArea(c)
		weed_area=[]
		weed_area.append(area)
		max_weed_area=max(weed_area)
		#print(weed_area,"max and min area: ",max(weed_area),min(weed_area))
		print("max weed area",max_weed_area)
		# when  larger weed detected in the crop bed, run detection process 
		if area>200 and area<1000: # if area >800
			# big weeds only be detected per frame
			print("detected weed area",area)
			# calculate centroid of the weed detected  using .moment()
			M = cv2.moments(c)
			cx = int(M["m10"] / M["m00"])
			cy = int(M["m01"] / M["m00"])
			print('2D weed centroids coordinate',cx,cy)#check point
			# draw contours
			cv2.drawContours(color_image,[c],-1,(0,0,240),2)  
			cv2.circle(color_image,(cx,cy),14,(0,0,240),-1) 

			depth_measurement = depth_image[cx, cy] 
			# by defalult in 400 series depth scale =.001m
			depth_scale=.0001# not .001
			depth=depth_measurement*depth_scale; print(f'obj({cx},{cy}) depth in meter= {depth} OR {depth*105}cm')



	# Get the depth measurement using pixel co-ord

	#y=int(340);x=int(275) # pixel coordinate to get the depth of it

	#dpt_frame=depth_frame.as_depth_frame()
	#depth_measurement2=dpt_frame.get_distance(x,y)
	#print(depth_measurement2)
	#depth_measurement = depth_image[cx, cy] 
	# by defalult in 400 series depth scale =.001m
	#depth_scale=.0001# not .001
	#depth=depth_measurement*depth_scale; print(f'obj({cx},{cy}) depth in meter= {depth} OR {depth*105}cm')

	# break the stream upon press 'q'
	if cv2.waitKey(1)==ord('q'):
		break;print('stream interrupted')
	# store last frame 
	#cv2.imwrite(os.path.join(stored_img_path, f'{Img_dir}.jpg'),Img);print('Image stored')
	#H,W=color_image.shape #_{H}:{W}
	cv2.imwrite(f'RGB.jpg', color_image)
	cv2.imwrite('Depth_cm.jpg',depth_cm)
# Stop the pipeline
pipeline.stop()

