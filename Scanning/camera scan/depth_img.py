''' intel D05 - 1280x720 ; 7 cm to 50cm= f .1mm obj detection at 7 cm; 90 fps
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
	

	y=int(320);x=int(240) # pixel coordinate to get the depth of it
	color_image = cv2.circle(color_image, (y,x),25,(0,255,0),1) 
	# show frames using cv2
	cv2.imshow('RGB', color_image);
	cv2.imshow('Depth',depth_image)
	cv2.imshow('Depth color map',depth_cm)


	# Get the depth measurement using pixel co-ord
	#dpt_frame=depth_frame.as_depth_frame()
	#depth_measurement2=dpt_frame.get_distance(x,y)
	#print(depth_measurement2)
	depth_measurement = depth_image[x, y] 
	# by defalult in 400 series depth scale =.001m
	depth_scale=.0001# not .001
	depth=depth_measurement*depth_scale; print(f'obj({x},{y}) depth in meter= {depth} OR {depth*95}cm')

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

