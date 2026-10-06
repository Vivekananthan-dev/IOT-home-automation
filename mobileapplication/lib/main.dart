import 'package:firebase_core/firebase_core.dart';
//import 'package:mobileapplication/screens/home_screen.dart';
import 'package:mobileapplication/screens/signin_screen.dart';
import 'package:flutter/material.dart';

void main() async {
  
  await Firebase.initializeApp(
    options: const FirebaseOptions(
    apiKey: '', 
    appId: '', 
    messagingSenderId: '', 
    projectId: '',
    authDomain: '',
    storageBucket: '',
    databaseURL: '',
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
