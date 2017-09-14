# QtNets

An open source library for the visualization of complex graphs


### Prerequisites

To run all the software modules of this project, first you need to install the Qt development environment. You can download it here (https://www1.qt.io/download/)


### Installing

Launch the build.sh script to build the entire project; it accepts one parmater only to define the optimization level: it could be **Debug** or **Release**.
The first time you launch the script, it will generate a configuration file (_qtnets.conf_) you need to edit in order to provide the build system with the correct installation paths.
Here is an example of the default _qtnets.conf_ file:
```
#!/bin/sh

export QMAKE="/home/angelo/Qt/5.7/gcc_64/bin/qmake"
export PROJ_ROOT="/home/angelo/Projects/QtNets"
export APP_PROJ_DIR="$PROJ_ROOT/Projects"
export LIB_PROJ_DIR="$PROJ_ROOT/Projects/Lib"
export PLUGIN_PROJ_DIR="$PROJ_ROOT/Projects/Plugin"

export ROOT_OUT_DIR="$PROJ_ROOT/OUT"
export APP_OUT_DIR="$ROOT_OUT_DIR/$MODE"
export LIB_OUT_DIR="$APP_OUT_DIR/Libs"
export PLUGIN_OUT_DIR="$APP_OUT_DIR/Plugins"
export INCLUDE_OUT_DIR="$APP_OUT_DIR/Include"

export SHARED_ROOT="$PROJ_ROOT/Shared"
```

after the build process is done, you will find following resources:
```
OUT/$1/Libs/
OUT/$1/Plugins/
OUT/$1/Include/
OUT/$1/QtNetsEditor
OUT/$1/QtNetsTest
OUT/$1/QtNetsBenchmark
OUT/$1/profile.sh
OUT/$1/launch.sh
OUT/$1/test.sh
OUT/$1/benchmark.sh
```

**Note**: this build system is guaranteed to work only on linux-based environments.

## Running the tests

After the project has been built (see the Installing section), simply run the **OUT/$1/test.sh** script in your sheel; it will execute the unit tests contained in the _OUT/$1/QtNetsTest_ application and print the result on stdout.

## Deployment

Add additional notes about how to deploy this on a live system

## Built With

* [Dropwizard](http://www.dropwizard.io/1.0.2/docs/) - The web framework used
* [Maven](https://maven.apache.org/) - Dependency Management
* [ROME](https://rometools.github.io/rome/) - Used to generate RSS Feeds

## Contributing

Please read [CONTRIBUTING.md](https://gist.github.com/PurpleBooth/b24679402957c63ec426) for details on our code of conduct, and the process for submitting pull requests to us.

## Versioning

We use [SemVer](http://semver.org/) for versioning. For the versions available, see the [tags on this repository](https://github.com/your/project/tags). 

## Authors

* **Angelo Prudentino** - *Design and development* - [angeloprudentino](https://github.com/angeloprudentino)

## License

This project is licensed under the GNU General Public License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

* I'd like to thank my tutors for this thesis project: Prof. [Stefano Di Carlo](https://www.testgroup.polito.it/stefano-di-carlo/) and Eng. [Alessandro Savino](https://www.testgroup.polito.it/alessandro-savino/). They always helped me during hard times and guided me to the final goal.
