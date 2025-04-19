import 'package:firebase_auth/firebase_auth.dart';
import 'package:firebase_database/firebase_database.dart';
import 'package:flutter/material.dart';
import 'package:mobileapplication/screens/signin_screen.dart';
import 'package:mobileapplication/screens/webpage.dart';
import 'package:mobileapplication/screentest/door_control_page.dart';
import 'package:mobileapplication/temperature/temperaturescreen.dart';
import 'package:mobileapplication/utils/color_utils.dart';
//import 'package:mobileapplication/screentest/monitor_page.dart';

class HomeScreen extends StatefulWidget {
  const HomeScreen({super.key});

  @override
  _HomeScreenState createState() => _HomeScreenState();
}

class _HomeScreenState extends State<HomeScreen> {
  late DatabaseReference _devicesRef;
  int switch1 = 0;
  int switch2 = 0;
  int switch3 = 0;

@override
void initState() {
  super.initState();
  _devicesRef = FirebaseDatabase.instance.ref('devices');
  _devicesRef.onValue.listen((event) {
    final data = event.snapshot.value;
    if (data != null && data is Map<dynamic, dynamic>) {
      setState(() {
        switch1 = _getIntValueFromData(data, 'light1') ?? 0;
        switch2 = _getIntValueFromData(data, 'light2') ?? 0;
        switch3 = _getIntValueFromData(data, 'fan') ?? 0;
      });
    } else {
      // Handle the case when the data is null or not a Map
      print('Invalid data from Firebase: $data');
    }
  });
}

int? _getIntValueFromData(Map<dynamic, dynamic> data, String key) {
  final value = data[key];
  if (value is int) {
    return value;
  } else if (value is String) {
    return int.tryParse(value);
  }
  return null;
}

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Home Page'),
        actions: [
          IconButton(
            icon: const Icon(Icons.logout),
            onPressed: () {
              FirebaseAuth.instance.signOut().then((value) {
                print("Signed Out");
                Navigator.pushReplacement(
                  context,
                  MaterialPageRoute(
                    builder: (context) => const SignInScreen(),
                  ),
                );
              });
            },
          ),
        ],
        leading: IconButton(
          icon: const Icon(Icons.arrow_back),
          onPressed: () {
            Navigator.of(context).pop();
          },
        ),
      ),
      body: Container(
          decoration: BoxDecoration(
              gradient: LinearGradient(colors: [
            hexStringToColor("#D16BA5"),
            hexStringToColor("#86A8E7"),
            hexStringToColor("#5FFBF1")
          ],
      )
      ),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.center,
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            const Text(
              'Welcome to Home',
              style: TextStyle(
                fontSize: 24,
                fontWeight: FontWeight.bold,
                color: Colors.white,
              ),
            ),
            const SizedBox(height: 16),
            Expanded(
              child: Column(
                mainAxisAlignment: MainAxisAlignment.center,
                children: [
                  Row(
                    mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                    children: [
                      _buildButtonWithState(
                        'assets/light.png',
                        'Light 1',
                        switch1,
                        () {
                          _updateSwitchState('light1', 1);
                        },
                        () {
                          _updateSwitchState('light1', 0);
                        },
                      ),
                      _buildButtonWithState(
                        'assets/light.png',
                        'Light 2',
                        switch2,
                        () {
                          _updateSwitchState('light2', 1);
                        },
                        () {
                          _updateSwitchState('light2', 0);
                        },
                      ),
                    ],
                  ),
                  const SizedBox(height: 16),
                  Row(
                    mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                    children: [
                      _buildButtonWithState(
                        'assets/fan.png',
                        'Fan',
                        switch3,
                        () {
                          _updateSwitchState('fan', 1);
                        },
                        () {
                          _updateSwitchState('fan', 0);
                        },
                      ),
                      _buildRedirectButton('assets/door.png', 'Door', () {
                        Navigator.push(
                            context,
                            MaterialPageRoute(
                                builder: (context) => const DoorControlPage()));
                      }),
                    ],
                  ),
                  const SizedBox(height: 16),
                  Row(
                    mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                    children: [
                      _buildRedirectButton('assets/monitor.png', 'Readings', () {
                        Navigator.push(
                            context,
                            MaterialPageRoute(
                                builder: (context) => const SensorPage()));
                      }),
                      _buildRedirectButton('assets/cctv.png', 'Monitor', () {
                        Navigator.push(
                          context,
                          MaterialPageRoute(
                            builder: (context) => const WebsitePage(url: 'http://127.0.0.1:8081'),
                          ),
                        );

                        // Navigate to temperature monitor page
                      }),
                    ],
                  ),
                ],
              ),
            ),
          ],
        ),
      ),
    );
  }

  Widget _buildButtonWithState(String imagePath, String label, int state,
      VoidCallback onPressedOn, VoidCallback onPressedOff) {
    return Column(
      children: [
        Material(
          color: Colors.transparent,
          child: InkWell(
            onTap: state == 0 ? onPressedOn : onPressedOff,
            child: Padding(
              padding: const EdgeInsets.all(8.0),
              child: Image.asset(
                imagePath,
                width: 64,
                height: 64,
              ),
            ),
          ),
        ),
        const SizedBox(height: 6),
        Text(
          label,
          style: const TextStyle(fontSize: 16),
        ),
      ],
    );
  }

  Widget _buildRedirectButton(
      String imagePath, String label, VoidCallback onPressed) {
    return Column(
      children: [
        Material(
          color: Colors.transparent,
          child: InkWell(
            onTap: onPressed,
            child: Padding(
              padding: const EdgeInsets.all(8.0),
              child: Image.asset(
                imagePath,
                width: 64,
                height: 64,
              ),
            ),
          ),
        ),
        const SizedBox(height: 6),
        Text(
          label,
          style: const TextStyle(fontSize: 16),
        ),
      ],
    );
  }

  void _updateSwitchState(String device, int value) {
    _devicesRef.child(device).set(value);
  }
}
