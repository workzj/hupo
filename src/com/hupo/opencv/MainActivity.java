package com.hupo.opencv;

import java.io.BufferedReader;
import java.io.File;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;
import org.opencv.android.OpenCVLoader;
import android.app.Activity;
import android.os.Bundle;
import android.os.Environment;
import android.widget.TextView;

import com.example.opencv.R;

public class MainActivity extends Activity {

	static{
			if (!OpenCVLoader.initDebug()) {
		        // Handle initialization error
		    }else{
		    	System.loadLibrary("python2.7");
		    	System.loadLibrary("OpencvHeader");
		    }
		}
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		int iResult = new LibOpencvHeader().Add(5, 6);
		TextView tv = (TextView)this.findViewById(R.id.text1);
		String out = "";//getExtSDCardPath().get(0);
		String sdcard = Environment.getExternalStorageDirectory().getPath();
		
		String output = "outside sdcard Path : " + out;
		output += "\\t\\n";
		output += "inside sdcard Path : " + sdcard;
		output += "\\t\\n";
		output += Integer.toString(iResult);
		
		tv.setText(output);
		//setContentView(tv);
	}
	
	public List<String> getExtSDCardPath()  
    {  
        List<String> lResult = new ArrayList<String>();  
        try {  
            Runtime rt = Runtime.getRuntime();  
            Process proc = rt.exec("mount");  
            InputStream is = proc.getInputStream();  
            InputStreamReader isr = new InputStreamReader(is);  
            BufferedReader br = new BufferedReader(isr);  
            String line;  
            while ((line = br.readLine()) != null) {  
                if (line.contains("extSdCard"))  
                {  
                    String [] arr = line.split(" ");  
                    String path = arr[1];  
                    File file = new File(path);  
                    if (file.isDirectory())  
                    {  
                        lResult.add(path);  
                    }  
                }  
            }  
            isr.close();  
        } catch (Exception e) {  
        }  
        return lResult;  
    }  
}
