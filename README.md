# REDHAWK Basic Components rh.SourceVITA49
 
## Description

Contains the source and build script for the REDHAWK Basic Component
rh.SourceVITA49. The rh.SourceVITA49 component connects to a UDP/multicast or
TCP VITA49 packet stream and converts the headers to SRI Keywords and data to
the BULKIO interface of the user's choice for use within REDHAWK domain
applications.
 
## Branches and Tags

All REDHAWK core assets use the same branching and tagging policy. Upon release,
the `master` branch is rebased with the specific commit released, and that
commit is tagged with the asset version number. For example, the commit released
as version 1.0.0 is tagged with `1.0.0`.

Development branches (i.e. `develop` or `develop-X.X`, where *X.X* is an asset
version number) contain the latest unreleased development code for the specified
version. If no version is specified (i.e. `develop`), the branch contains the
latest unreleased development code for the latest released version.

## REDHAWK Version Compatibility

| Asset Version | Minimum REDHAWK Version Required |
| ------------- | -------------------------------- |
| 3.x           | 2.0                              |
| 2.x           | 1.10                             |
| 1.1.x         | 1.8.6                            |

## Installation Instructions
This asset requires the rh.VITA49 shared library. This shared library  must be
installed in order to build and run this asset. To build from source, run the
`build.sh` script found at the top level directory. To install to $SDRROOT, run
`build.sh install`. Note: root privileges (`sudo`) may be required to install.
 
## Copyrights

This work is protected by Copyright. Please refer to the
[Copyright File](COPYRIGHT) for updated copyright information.

## License

REDHAWK Basic Components rh.SourceVITA49 is licensed under the GNU Lesser
General Public License (LGPL).

