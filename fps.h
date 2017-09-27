//taken from http://procedurallygenerated.com/?p=158

#define FPS_MAX 60

struct fps_data
{Uint32 frame_min;
 Uint32 frame_max;
 double frame_average;
 int fps;
}fps_data;

////////////////////////////////////////////////////////////////////////////////
//Name:    FPS
//Purpose: FPS manager
////////////////////////////////////////////////////////////////////////////////
class fps
{float max_frame_ticks;
 Uint32 last_second_ticks;
 int frame_count;
 //--//
 Uint32 min_ticks;
 Uint32 max_ticks;
 double average_ticks;
 Uint32 last_frame_ticks;
 //--//
 Uint32 current_ticks;
 Uint32 target_ticks;
 
////////////////////////////////////////////////////////////////////////////////
 public:
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//Name:    Init
//Purpose: Initialize FPS Manager
//Notes:   The +0.00001 on max_frame_ticks is to offset the losses from rounding
//         Otherwise we get 61/60 fps
////////////////////////////////////////////////////////////////////////////////
 void init()
 {max_frame_ticks=(1000.0/(float)FPS_MAX)+0.00001;
  frame_count=0;
  last_second_ticks=SDL_GetTicks();
 }
 
////////////////////////////////////////////////////////////////////////////////
//Name:    Limit
//Purpose: Limit FPS to FPS_MAX
////////////////////////////////////////////////////////////////////////////////
 bool limit()
 {frame_count++;
  target_ticks = last_second_ticks + Uint32(frame_count * max_frame_ticks);
  current_ticks = SDL_GetTicks();
  //--//
  average_ticks+=current_ticks-last_frame_ticks;
  if(current_ticks-last_frame_ticks<=min_ticks)
    {min_ticks=current_ticks-last_frame_ticks;
    }
  if(current_ticks-last_frame_ticks>=max_frame_ticks)
    {max_ticks=current_ticks-last_frame_ticks;
    }
  //--//
  if(current_ticks<target_ticks)
    {SDL_Delay(target_ticks-current_ticks);
     current_ticks = SDL_GetTicks();
    }
  //--//
  last_frame_ticks=current_ticks;
  //--// 
  if(current_ticks-last_second_ticks>=1000)
    {fps_data.fps=frame_count;
     fps_data.frame_average=average_ticks/frame_count;
     fps_data.frame_min=min_ticks;
     fps_data.frame_max=max_ticks;
     //--//
     frame_count=0;
     min_ticks=1000;
     max_ticks=0;
     average_ticks=0;
     last_second_ticks=SDL_GetTicks();
     return true;
    }
  //--//
  return false;
 }
////////////////////////////////////////////////////////////////////////////////
}fps;

