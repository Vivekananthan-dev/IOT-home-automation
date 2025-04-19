import 'package:flutter/material.dart';
import 'package:firebase_database/firebase_database.dart';
import 'package:mobileapplication/utils/color_utils.dart';

class DoorControlPage extends StatefulWidget {
  const DoorControlPage({super.key});

  @override
  // ignore: library_private_types_in_public_api
  _DoorControlPageState createState() => _DoorControlPageState();
}

class _DoorControlPageState extends State<DoorControlPage> {
  final TextEditingController _usernameController = TextEditingController();
  final TextEditingController _passwordController = TextEditingController();

  late DatabaseReference _databaseRef;

  @override
  void initState() {
    super.initState();
    _databaseRef = FirebaseDatabase.instance.ref();
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Door Control'),
      ),
      body: Container(
          decoration: BoxDecoration(
              gradient: LinearGradient(colors: [
            hexStringToColor("#D16BA5"),
            hexStringToColor("#86A8E7"),
            hexStringToColor("#5FFBF1")
          ],
          ),
        ),
        child: Padding(
          padding: const EdgeInsets.all(20.0),
          child: Column(
            crossAxisAlignment: CrossAxisAlignment.center,
            children: [
              TextField(
                controller: _usernameController,
                decoration: const InputDecoration(
                  labelText: 'Username',
                ),
              ),
              const SizedBox(height: 20),
              TextField(
                controller: _passwordController,
                obscureText: true,
                decoration: const InputDecoration(
                  labelText: 'Password',
                ),
              ),
              const SizedBox(height: 20),
              Row(
                mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                children: [
                  ElevatedButton(
                    onPressed: () {
                      _authenticateAndControlDoor(openDoor: 1);
                    },
                    child: const Text('Open Door'),
                  ),
                  ElevatedButton(
                    onPressed: () {
                      _authenticateAndControlDoor(openDoor: 2);
                    },
                    child: const Text('Close Door'),
                  ),
                ],
              ),
            ],
          ),
        ),
      ),
    );
  }

  void _authenticateAndControlDoor({required int openDoor}) {
    // Authenticate the user here, for simplicity we'll just compare the entered username and password
    if (_usernameController.text == 'vs' &&
        _passwordController.text == 'vs') {
      // If authentication is successful, update the door status in the database
      _databaseRef.child('devices/door').set(openDoor);
      ScaffoldMessenger.of(context).showSnackBar(
        SnackBar(
          content: Text(openDoor == 1  ? 'Door Opened' : 'Door Closed'),
        ),
      );
    } else {
      ScaffoldMessenger.of(context).showSnackBar(
        const SnackBar(
          content: Text('Incorrect username or password'),
        ),
      );
    }
  }
}
