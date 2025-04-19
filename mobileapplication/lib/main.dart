import 'package:firebase_core/firebase_core.dart';
//import 'package:mobileapplication/screens/home_screen.dart';
import 'package:mobileapplication/screens/signin_screen.dart';
import 'package:flutter/material.dart';

void main() async {
  
  await Firebase.initializeApp(
    options: const FirebaseOptions(
    apiKey: 'AIzaSyDzL_-th-PXinHPlp6PapqB8cq_MTWTS04', 
    appId: '1:1024911641176:android:078415cf1965f283db582d', 
    messagingSenderId: '1024911641176', 
    projectId: 'iot-app-f5138',
    authDomain: 'iot-app-f5138.firebaseapp.com',
    storageBucket: 'iot-app-f5138.appspot.com',
    databaseURL: 'https://iot-app-f5138-default-rtdb.asia-southeast1.firebasedatabase.app/',
    )
  );
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});
 

  // This widget is the root of your application.
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Mobile App',
      theme: ThemeData(
        primarySwatch: Colors.blue,
      ),
      home: const SignInScreen(),
      //home: const HomeScreen(),
    );
  }
}