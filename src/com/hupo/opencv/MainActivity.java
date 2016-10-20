package com.hupo.opencv;

import java.io.DataOutputStream;

import org.opencv.android.OpenCVLoader;
import android.app.Activity;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
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
	/**
     * Ӧ�ó������������ȡ RootȨ�ޣ��豸�������ƽ�(���ROOTȨ��)
     * @param command ���String apkRoot="chmod 777 "+getPackageCodePath(); RootCommand(apkRoot);
     * @return Ӧ�ó�����/���ȡRootȨ��
     */
    public static boolean RootCommand(String command)
    {
        Process process = null;
        DataOutputStream os = null;
        try
        {
            process = Runtime.getRuntime().exec("su");
            os = new DataOutputStream(process.getOutputStream());
            os.writeBytes(command + "\n");
            os.writeBytes("exit\n");
            os.flush();
            process.waitFor();
        } catch (Exception e)
        {
            Log.d("*** DEBUG ***", "ROOT REE" + e.getMessage());
            return false;
        } finally
        {
            try
            {
                if (os != null)
                {
                    os.close();
                }
                process.destroy();
            } catch (Exception e)
            {
            }
        }
        Log.d("*** DEBUG ***", "Root SUC ");
        return true;
    }
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		String apkRoot="chmod 777 "+getPackageCodePath();
        RootCommand(apkRoot);
		int iResult = new LibOpencvHeader().Add(5, 6);
		TextView tv = (TextView)this.findViewById(R.id.text1);
		
		String output = "getPackageCodePath : " + getPackageCodePath();;
		output += "\\t\\n";
		output += "the result : " + Integer.toString(iResult);
		
		tv.setText(output);
	}
	
}
