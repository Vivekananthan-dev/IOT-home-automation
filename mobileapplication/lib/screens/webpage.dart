import 'package:flutter/material.dart';
import 'package:url_launcher/url_launcher.dart';

class WebsitePage extends StatelessWidget {
  final String url;

  const WebsitePage({super.key, required this.url});

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Website Page'),
      ),
      body: Center(
        child: ElevatedButton(
          onPressed: () {
            _launchURL(url);
          },
          child: const Text('Visit Website'),
        ),
      ),
    );
  }

  void _launchURL(String url) async {
    if (await canLaunch(url)) {
      await launch(url);
    } else {
      throw 'Could not launch $url';
    }
  }
}
