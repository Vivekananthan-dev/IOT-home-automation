import 'package:flutter/material.dart';
import 'package:firebase_database/firebase_database.dart';

class SensorPage extends StatefulWidget {
  const SensorPage({super.key});

  @override
  // ignore: library_private_types_in_public_api
  _SensorPageState createState() => _SensorPageState();
}

class _SensorPageState extends State<SensorPage> {
  final _databaseRef = FirebaseDatabase.instance.ref('sensors');
  List<double> _temperatureData = [];
  List<double> _currentData = [];

  @override
  void initState() {
    super.initState();
    _listenForValues();
  }

  Future<void> _listenForValues() async {
    // Listen for temperature changes
    _databaseRef.child('temperature').onValue.listen((event) {
      final value = event.snapshot.value;
      if (value != null) {
        final temperature = double.tryParse(value.toString());
        if (temperature != null) {
          setState(() {
            _temperatureData.add(temperature);
          });
        }
      }
    });

    // Listen for current changes
    _databaseRef.child('current').onValue.listen((event) {
      final value = event.snapshot.value;
      if (value != null) {
        final current = double.tryParse(value.toString());
        if (current != null) {
          setState(() {
            _currentData.add(current);
          });
        }
      }
    });
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Sensor Monitor'),
        actions: [
          IconButton(
            icon: const Icon(Icons.delete),
            onPressed: () {
              setState(() {
                _temperatureData.clear();
                _currentData.clear();
              });
            },
          ),
        ],
      ),
      body: Center(
        child: SingleChildScrollView(
          child: Padding(
            padding: const EdgeInsets.all(16.0),
            child: Column(
              mainAxisAlignment: MainAxisAlignment.center,
              children: [
                const Text(
                  'Temperature',
                  style: TextStyle(fontSize: 20, fontWeight: FontWeight.bold),
                ),
                const SizedBox(height: 10),
                _buildDataList(_temperatureData, '°C'),
                const SizedBox(height: 20),
                const Text(
                  'Current',
                  style: TextStyle(fontSize: 20, fontWeight: FontWeight.bold),
                ),
                const SizedBox(height: 10),
                _buildDataList(_currentData, 'A'),
              ],
            ),
          ),
        ),
      ),
    );
  }

  Widget _buildDataList(List<double> data, String unit) {
  return SizedBox(
    height: 200,
    child: Center(
      child: ListView.builder(
        itemCount: data.length,
        itemBuilder: (context, index) {
          return Text(
            '${data[index].toStringAsFixed(1)} $unit',
            style: const TextStyle(fontSize: 16),
            textAlign: TextAlign.center, // Add this line to center the text
          );
        },
      ),
    ),
  );
}
}