package ru.maximoff.zasample;

import android.app.Activity;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.Toast;
import java.io.File;
import java.util.ArrayList;
import java.util.List;
import ru.maximoff.zipalign.ZipAligner;
import android.preference.PreferenceManager;
import android.content.SharedPreferences;

public class MainActivity extends Activity {
	private final int REQUEST = 1010;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
		final SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(this);
		final EditText path = findViewById(R.id.mainEditText1);
		path.setText(prefs.getString("saved_path", ""));
		final CheckBox sharedLibs = findViewById(R.id.mainCheckBox1);
		Button align = findViewById(R.id.mainButton1);
		Button check = findViewById(R.id.mainButton2);
		OnClickListener listener = new OnClickListener() {
			@Override
			public void onClick(View p1) {
				String filePath = path.getText().toString();
				if (filePath.equals("")) {
					return;
				}
				File file = new File(filePath);
				if (!file.exists()) {
					st("File " + filePath + " not found!");
					return;
				}
				prefs.edit().putString("saved_path", filePath).commit();
				boolean resultOk;
				switch (p1.getId()) {
					case R.id.mainButton1:
						int p = file.getName().lastIndexOf(".");
						String target;
						if (p > 0){
							target = file.getParent() + "/" + file.getName().substring(0, p) +"_aligned" + file.getName().substring(p);
						} else {
							target = filePath + "_aligned";
						}
						resultOk = ZipAligner.align(filePath, target, ZipAligner.DEFAULT_LEVEL, sharedLibs.isChecked());
						path.setText(target);
						break;

					case R.id.mainButton2:
						resultOk = ZipAligner.isAligned(filePath, ZipAligner.DEFAULT_LEVEL, sharedLibs.isChecked());
						break;

					default:
						resultOk = false;
						break;
				}
				st("Result: " + String.valueOf(resultOk));
			}
		};
		align.setOnClickListener(listener);
		check.setOnClickListener(listener);
		checkPerm(this, new String[]{"READ_EXTERNAL_STORAGE", "WRITE_EXTERNAL_STORAGE"});
    }

	@Override
	public void onRequestPermissionsResult(int requestCode,  String[] permissions,  int[] grantResults) {
		switch (requestCode) {
			case REQUEST:
				if (grantResults.length > 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
					st("Permissions granted!");
				} else if (!shouldShowRequestPermissionRationale(permissions[0])) {
					st("This app needs permissions!");
					goToSettings();
					finish();
				} else {
					checkPerm(this, new String[]{"READ_EXTERNAL_STORAGE", "WRITE_EXTERNAL_STORAGE"});
				}
				break;

			default:
				super.onRequestPermissionsResult(requestCode, permissions, grantResults);
		}
	}

	private void goToSettings() {
		Intent myAppSettings = new Intent(android.provider.Settings.ACTION_APPLICATION_DETAILS_SETTINGS, Uri.parse("package:" + getPackageName()));
		startActivity(myAppSettings);
	}

	public void st(String s) {
		Toast.makeText(this, s, Toast.LENGTH_SHORT).show();
	}

	public void checkPerm(Activity act, String[] groups) {
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
			List<String> permissions = new ArrayList<>();
			for (String permission : groups) {
				permission = "android.permission." + permission;
				if (act.checkSelfPermission(permission) != PackageManager.PERMISSION_GRANTED) {
					permissions.add(permission);
				}
			}
			if (permissions.isEmpty()) {
				return;
			}
			String[] request = permissions.toArray(new String[permissions.size()]);
			act.requestPermissions(request, REQUEST);
		}
	}
}
