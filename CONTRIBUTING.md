# Guidelines for contributing to Sensor PCB HAT project EEE3088F 2023

This project will not be actively monitored once it is concluded in May 2023. However, contributions are still welcome, in the spirit of making this project more useful for general contributors.

## General guidelines for contribution

Contributions to this project can be made direcly on the github page in the form of issues or pull reqeusts. In the instance that a pressing contribution is not being attended to by the core project team, the team can be emailed directly on dgmrob001@myuct.ac.za, clrcam007@myuct.ac.za, or frskia001@myuct.ac.za.

## Issues

### Create a new issue

Issues can be created on the github. Make sure to explain your issues thoroughly, referencing:
- What the issue is / why it is a problem
- Under what circumstances is arises
- Any other details that could be useful when replicating the issue

### Solve an issue

If you take an interest in solving any issue, feel free to work on changes to fix the issue at hand, and suggest these changes following the guidelines in the next section.

## Suggest changes

### Guideline for submitting change requests

The core project team will not be actively monitoring the project once it is concluded in May 2023. Thus, changes which include work on the project design files are welcome, but purly theoretical changes (i.e., requests for the project team to work on a specific aspect of the project) are not generally useful.

Suggested changes can be submitted as pull requests, and should include thorough explanations about:

- The reason for the changes
- The way in which the change has been implemented

### Guidelines for hardware changes

Hardware changes should be focused on improving the existing hardware design, and not on adding additional functionality.

Examples of good hardware change suggestions:
- "X component is no longer being manufactured, and can be replaced by Y component instead"
- "The PCB design creates X electrical/mechanical issue, and should be changed in Y way"

Examples of poor hardware changes suggestions:
- "It would be useful to include X sensor in addition to the current sensors on the HAT"
- "Can the HAT be reconfigured to be powered from AA batteries?"

Hardware changes should add to the current KiCad project files, and should not be written using other, incompatible software.

### Guidelines for software changes

Software changes that add any sort of board functionality are welcome. Changes should not remove any existing functionality, unless it is replaced by something more useful/versatile, in which case a thorough explanation must be given to justify the removal of the functionality.

It is recommended that software be develop using STM's STM32 Cube IDE, following all guidelines for the usage of the IDE, to ensure compatibility with existing software and with the microcontroller.
