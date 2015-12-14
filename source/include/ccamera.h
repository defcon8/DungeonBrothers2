#ifndef CCAMERA_H
#define CCAMERA_H

class cCamera
{
	public:
		
		
		int up, right, down, left, none, direction, velocity, x, y, dty, dtx, pos;
		cCamera();
		~cCamera();
		void cameraMovement();
			
	protected:
};

#endif
