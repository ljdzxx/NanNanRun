/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2014 Chukong Technologies Inc.
 
http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package org.cocos2dx.cpp;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.*;
import android.os.Bundle;
//import android.view.*;

public class AppActivity extends Cocos2dxActivity {
	
	private Cocos2dxGLSurfaceView glSurfaceView;

    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState); 
        //this.hideSystemUI();
    }
    
    /*protected void onResume()
    {
    	super.onResume();
    	this.hideSystemUI();
    }*/

	/*public boolean dispatchKeyEvent(KeyEvent event)
	{
		Log.i("cp", "dispatchKeyEvent");
		setFullScreen();
		if (event.getAction() == KeyEvent.ACTION_MULTIPLE)
			return mUnityPlayer.onKeyMultiple(event.getKeyCode(), event.getRepeatCount(), event);
		return super.dispatchKeyEvent(event);
	}*/

    public Cocos2dxGLSurfaceView onCreateView()
    {
        glSurfaceView = new Cocos2dxGLSurfaceView(this);

        this.hideSystemUI();

        // create stencil buffer
        glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);

        return glSurfaceView;
    }

    public void onWindowFocusChanged(boolean hasFocus)
    {
        super.onWindowFocusChanged(hasFocus);
        if (hasFocus)
        {
            this.hideSystemUI();
        }
    }

    private void hideSystemUI()
    {
        // Set the IMMERSIVE flag.
        // Set the content to appear under the system bars so that the content
        // doesn't resize when the system bars hide and show.
    	if(android.os.Build.VERSION.SDK_INT>18){//安卓4.4以上
	        glSurfaceView.setSystemUiVisibility(
	                Cocos2dxGLSurfaceView.SYSTEM_UI_FLAG_LAYOUT_STABLE 
	                | Cocos2dxGLSurfaceView.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
	                | Cocos2dxGLSurfaceView.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
	                | Cocos2dxGLSurfaceView.SYSTEM_UI_FLAG_HIDE_NAVIGATION // hide nav bar
	                | Cocos2dxGLSurfaceView.SYSTEM_UI_FLAG_FULLSCREEN // hide status bar
	                | Cocos2dxGLSurfaceView.SYSTEM_UI_FLAG_IMMERSIVE_STICKY);
    	}
    }
    
   

    static
    {
        System.loadLibrary("cocos2dcpp");
    }
	
	
	
    
    /*
    public void setFullScreen()  
    {  
          
        //开启全屏  
        View m_DecorView=getWindow().getDecorView();  
        if(getSystemVersion()>18)  
        {  
            m_DecorView.setSystemUiVisibility(
                    View.SYSTEM_UI_FLAG_LAYOUT_STABLE  
                    | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION  
                    | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN  
                    | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION  
                    | View.SYSTEM_UI_FLAG_FULLSCREEN  
                    | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY  
                    );  
        }  
    }  
    
    public static int getSystemVersion()  
    {  
        int ver=android.os.Build.VERSION.SDK_INT;  
        return ver;  
    }
    */
    
}
