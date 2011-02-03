/*
 * FrameQImageConverter.h
 *
 *  Created on: Oct 15, 2010
 *      Author: aduda
 */

/*
 * This Class is Qt depended !!!
 * It can be used in Qt projects to convert base::samples::frame::Frame
 * to QImages
 *
 */

#ifndef FRAMEQIMAGECONVERTER_H_
#define FRAMEQIMAGECONVERTER_H_

#include <QtGui/QImage>
#include "FrameHelper.h"
#include <stdexcept>

class FrameQImageConverter
{
public:
  FrameQImageConverter(){};
  virtual
  ~FrameQImageConverter(){};

  //returns 1 if the size or format of dst has been changed  otherwise 0
 //int copyFrameToQImageRGB888(QImage &dst,base::samples::frame::frame_mode_t mode,int pixel_size, int width,int height,const char* pbuffer)
  //{
  int copyFrameToQImageRGB888(QImage &dst,base::samples::frame::frame_mode_t mode,int pixel_size, int width,int height, const char* pbuffer)
  {
    int ireturn = 0;
    //check if dst has the right format
    if(dst.width() != width || dst.height()!= height || dst.format() != QImage::Format_RGB888)
    {
        dst = QImage(width,height,QImage::Format_RGB888);
        ireturn = 1;
    }

    switch(mode)
    {
      case base::samples::frame::MODE_UNDEFINED:
        throw std::runtime_error(" FrameQImageConverter::copyFrameToQImageRGB888: Unknown frame mode!");
        break;

       case base::samples::frame::MODE_BAYER_RGGB:
       case base::samples::frame::MODE_BAYER_GRBG:
       case base::samples::frame::MODE_BAYER_BGGR:
       case base::samples::frame::MODE_BAYER_GBRG:
          FrameHelper::convertBayerToRGB24((const uint8_t*)pbuffer,(uint8_t*) dst.bits(), width, height ,mode);
       break;

	       case base::samples::frame::MODE_RGB:
          memcpy(dst.bits(),pbuffer,width*height*pixel_size);
          break;

       case base::samples::frame::MODE_UYVY:{
	
	  unsigned int theCounter = 0;	// just for testing
  	  unsigned int i,j;
	  uint8_t u,v,y1,y2,cb,cr,r1,r2,b1,b2,g1,g2;
          uint8_t* pbuffer1 = (uint8_t*) dst.bits();
          for(i = 0 ;i < height ;++i){
          	for(j = 0 ;j < width ;j+=2){
                        u      = pbuffer[(i*width*2)+j*2+0];
                        y1     = pbuffer[(i*width*2)+j*2+1];
                        v      = pbuffer[(i*width*2)+j*2+2];
                        y2      = pbuffer[(i*width*2)+j*2+3];
                  
			cb = u;
			cr = v;	
			// 	To convert uyvy image grayscale change r1-g2 vars to these
			//	r1 = y1;
			//	b1 = y1;
			//	g1 = y1;
			//	r2 = y2;
			//	b2 = y2;
			//	g2 = y2;

			
			//no-rounding conversion method
			r1 = (255/219)*(y1-16) + (255/112)*0.701*(cr-128);
			g1 = (255/219)*(y1-16) + (255/112)*0.886*(0.114/0.587)*(cb-128)-(255/112)*0.701*(0.299/0.587)*(cr-128);
			b1 = (255/219)*(y1-16) + (255/112)*0.886*(cb-128);
						
			r2 = (255/219)*(y2-16) + (255/112)*0.701*(cr-128);
			g2 = (255/219)*(y2-16) + (255/112)*0.886*(0.114/0.587)*(cb-128)-(255/112)*0.701*(0.299/0.587)*(cr-128);
			b2 = (255/219)*(y2-16) + (255/112)*0.886*(cb-128);



			// for testing
			//if(theCounter <10){
			//	printf("r1: %d g1: %d b1: %d\n",r1,g1,b1);
			//	printf("u: %d y1: %d v: %d y2: %d\n",u,y1,v,y2);
			//	}
                        pbuffer1[(i*width*3)+j*3+0] = r1;
                        pbuffer1[(i*width*3)+j*3+1] = g1;
                        pbuffer1[(i*width*3)+j*3+2] = b1;
                        pbuffer1[(i*width*3)+j*3+3] = r2;
                        pbuffer1[(i*width*3)+j*3+4] = g2;
                        pbuffer1[(i*width*3)+j*3+5] = b2;
                }
          }

	  //	to save a frame:a
	  //	QString fileName2("asd2.bmp");
	  //	dst.save(fileName2);	
	
	break;
	}
       case base::samples::frame::MODE_GRAYSCALE:
          //There is no conversion available by FrameHelper use qt
          //conversion --> one additional copy

          //check if buffer has the right format
          if(image_buffer.width() != width || image_buffer.height()!= height || image_buffer.format() != QImage::Format_Indexed8)
          {
             image_buffer = QImage(width,height,QImage::Format_Indexed8);
             for(int i = 0;i<256;++i)
               image_buffer.setColor(i,qRgb(i,i,i));
          }

          memcpy(image_buffer.bits(),pbuffer,width*height*pixel_size);
          dst = image_buffer.convertToFormat(QImage::Format_RGB888);
          break;

       default:
         throw std::runtime_error(" FrameQImageConverter::copyFrameToQImageRGB888: Can not convert frame to RGB888!");
    }
    return ireturn;
  };

  int copyFrameToQImageRGB888(QImage &dst,const base::samples::frame::Frame &frame)
  {
    return copyFrameToQImageRGB888(dst,
                      frame.frame_mode,
                      frame.pixel_size,
                      frame.getWidth(),
                      frame.getHeight(),
                      (const char*)frame.getImageConstPtr());
  };

  int copyFrameToQImageRGB888(QImage &dst,const QString &mode,int pixel_size, int width,int height, const char* pbuffer)
  {
    base::samples::frame::frame_mode_t _mode =  base::samples::frame::Frame::toFrameMode(mode.toStdString());
    return copyFrameToQImageRGB888(dst,_mode,pixel_size,width,height,pbuffer);
  };

private:
  QImage image_buffer;
};

#endif /* FRAMEQIMAGECONVERTER_H_ */