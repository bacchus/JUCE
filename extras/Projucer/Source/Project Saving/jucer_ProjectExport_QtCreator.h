/*
  ==============================================================================

   This file is part of the JUCE library.
   Copyright(c) 2015 - ROLI Ltd.

   Permission is granted to use this software under the terms of either:
   a) the GPL v2(or any later version)
   b) the Affero GPL v3

   Details of these licenses can be found at: www.gnu.org/licenses

   JUCE is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

   ------------------------------------------------------------------------------

   To release a closed-source product which uses JUCE, commercial licenses are
   available: visit www.juce.com for more information.

  ==============================================================================
*/
#pragma once

#include "jucer_ProjectExporter.h"

class QtCreatorProjectExporter: public ProjectExporter {
public:
    //==============================================================================
    // its not only Linux, its also Win and Mac, mb Android later
    static const char* getNameQtCreator()           { return "QtCreator"; }
    static const char* getValueTreeTypeName()   { return "QT_CREATOR"; }

    static QtCreatorProjectExporter* createForSettings(Project& project, const ValueTree& settings) {
        if (settings.hasType(getValueTreeTypeName()))
            return new QtCreatorProjectExporter(project, settings);

        return nullptr;
    }

    //==============================================================================
    QtCreatorProjectExporter(Project& p, const ValueTree& t)
        : ProjectExporter(p, t)
    {
        name = getNameQtCreator();

        if (getTargetLocationString().isEmpty())
            getTargetLocationValue() = getDefaultBuildsRootFolder() + "QtCreator";

        initialiseDependencyPathValues();
    }

    //==============================================================================
#if JUCE_LINUX || JUCE_WINDOWS || JUCE_MAC
    bool canLaunchProject() override            { return true; }
    bool launchProject() override               { return getProjectBundle().startAsProcess(); }
#else
    bool canLaunchProject() override            { return false; }//=0
    bool launchProject() override               { return false; }//=0
#endif // LWM

    //TODO: check this
    bool canCopeWithDuplicateFiles() override   { return false; }
    bool supportsUserDefinedConfigurations() const override { return true; }

#if JUCE_MAC
#define BCC_BOOL_MAC true
#else
#define BCC_BOOL_MAC false
#endif

#if JUCE_WINDOWS
#define BCC_BOOL_WIN true
#else
#define BCC_BOOL_WIN false
#endif

#if JUCE_LINUX
#define BCC_BOOL_LIN true
#else
#define BCC_BOOL_LIN false
#endif

    bool usesMMFiles() const override                   { return BCC_BOOL_MAC; }

    bool isXcode() const override                       { return false; } //TODO: compiler is in XCode
    bool isVisualStudio() const override                { return false; } //TODO: compiler is MSVC2015 so its VS
    bool isCodeBlocks() const override                  { return false; }
    bool isMakefile() const override                    { return false; }
    bool isAndroidStudio() const override               { return false; }
    bool isAndroidAnt() const override                  { return false; }
    //TODO: bool isQtCreator() const override           { return true; }

    bool isAndroid() const override                     { return false; }
    bool isWindows() const override                     { return BCC_BOOL_WIN; }
    bool isLinux() const override                       { return BCC_BOOL_LIN; }
    bool isOSX() const override                         { return BCC_BOOL_MAC; }
    bool isiOS() const override                         { return false; }

    //TODO: check support
    bool supportsVST() const override                   { return true; }
    bool supportsVST3() const override                  { return true; }
    bool supportsAAX() const override                   { return false; }
    bool supportsRTAS() const override                  { return false; }
    bool supportsAU()   const override                  { return false; }
    bool supportsAUv3() const override                  { return false; }
    bool supportsStandalone() const override            { return false; }


    Value getCppStandardValue()                 { return getSetting(Ids::cppLanguageStandard); }
    String getCppStandardString() const         { return getSettingString(Ids::cppLanguageStandard); }

//    Value getQtAppTypeStandardValue()           { return getSetting(Ids::qtAppType); }
//    String getQtAppTypeStandardString() const   { return settings[Ids::qtAppType]; }

    void createExporterProperties(PropertyListBuilder& properties) override {
        static const char* cppStandardNames[]  = { "C++03", "C++11", nullptr };
        static const char* cppStandardValues[] = { "-std=c++03", "-std=c++11", nullptr };
        properties.add(new ChoicePropertyComponent(
                           getCppStandardValue(),
                           "C++ standard to use",
                           StringArray(cppStandardNames),
                           Array<var> (cppStandardValues)),
                       "The C++ standard to specify in qt project");

//        static const char* qtAppTypeNames[]  = { "Default", "Console", nullptr };
//        static const char* qtAppTypeValues[] = { "app", "console", nullptr };
//        properties.add(new ChoicePropertyComponent(
//                           getQtAppTypeStandardValue(),
//                           "Qt app type",
//                           StringArray(qtAppTypeNames),
//                           Array<var> (qtAppTypeValues)),
//                       "Qt application type for qt project");
    }

    //==============================================================================
    void create(const OwnedArray<LibraryModule>&) const override {
        MemoryOutputStream mo;
        writeQtProject(mo);

        overwriteFileIfDifferentOrThrow(
                    getTargetFolder()
                    .getChildFile(project.getProjectFilenameRoot())
                    .withFileExtension(".pro"), mo);
    }

    //==============================================================================
    //TODO: check this for different platforms
    void addPlatformSpecificSettingsForProjectType (const ProjectType& type) override
    {
        makefileTargetSuffix = "";

#if JUCE_MAC
//        if (type.isStaticLibrary())
//            makefileTargetSuffix = ".a";
//        else if (type.isDynamicLibrary())
//            makefileTargetSuffix = ".so";
//        else if (type.isAudioPlugin())
//            makefileIsDLL = true;
#elif JUCE_WINDOWS
        if (type.isStaticLibrary())
            makefileTargetSuffix = ".lib";
        else if (type.isDynamicLibrary())
            makefileTargetSuffix = ".dll";
        else if (type.isAudioPlugin())
            makefileIsDLL = true;
#elif JUCE_LINUX
        if (type.isStaticLibrary())
            makefileTargetSuffix = ".a";
        else if (type.isDynamicLibrary())
            makefileTargetSuffix = ".so";
        else if (type.isAudioPlugin())
            makefileIsDLL = true;
#endif
    }

protected:
    //==============================================================================
    class QtBuildConfiguration: public BuildConfiguration {
    public:
        QtBuildConfiguration(Project& p, const ValueTree& settings, const ProjectExporter& e)
            : BuildConfiguration(p, settings, e)
        {
#if JUCE_LINUX
            setValueIfVoid(getLibrarySearchPathValue(), "/usr/X11R6/lib/");
#endif
        }

        Value getArchitectureType()             { return getValue(Ids::linuxArchitecture); } //TODO: check for architectures
        var getArchitectureTypeVar() const      { return config [Ids::linuxArchitecture]; }  //...on other platforms

        Value getWarningLevel()             { return getValue(Ids::warningLevel); }
        var getWarningLevelVar() const      { return config [Ids::warningLevel]; }

        var getDefaultOptimisationLevel() const override {
            return var((int)(isDebug() ? gccO0 : gccO3));
        }

        void createConfigProperties(PropertyListBuilder& props) override {
            addGCCOptimisationProperty(props);

            static const char* const archNames[] = { "(Default)", "<None>",       "32-bit(-m32)", "64-bit(-m64)", "ARM v6",       "ARM v7" };
            const var archFlags[]                = { var(),       var(String()), "-m32",         "-m64",           "-march=armv6", "-march=armv7" };

            props.add(new ChoicePropertyComponent(getArchitectureType(), "Architecture",
                                                  StringArray(archNames, numElementsInArray(archNames)),
                                                  Array<var>(archFlags, numElementsInArray(archFlags))));

            static const char* const warnNames[] = { "(Default)", "<None>", "<All>", "<Pedant>", "<Error>" };
            const var                warnFlags[] = { "",          "-w",     "-Wall", "-pedantic","-Werror" };

            props.add(new ChoicePropertyComponent(getWarningLevel(), "Warnings",
                                                  StringArray(warnNames, numElementsInArray(warnNames)),
                                                  Array<var>(warnFlags, numElementsInArray(warnFlags))));
        }
    };

    BuildConfiguration::Ptr createBuildConfig(const ValueTree& tree) const override {
        return new QtBuildConfiguration(project, tree, *this);
    }

private:
    //==============================================================================
    void initialiseDependencyPathValues() {
        vst3Path.referTo(Value(new DependencyPathValueSource(getSetting(Ids::vst3Folder),
                                                             Ids::vst3Path,
                                                             TargetOS::oslinux)));
    }

    File getProjectBundle() const {
        return getTargetFolder().getChildFile (project.getProjectFilenameRoot()).withFileExtension (".pro");
    }

    //==============================================================================
    void writeOutputDir(OutputStream& out, const BuildConfiguration& config) const {
        const String buildDirName("build/" + config.getName());
        const String intermediatesDirName("build/" + config.getName() + "/intermediate");
        String outputDir(buildDirName);

        if (config.getTargetBinaryRelativePathString().isNotEmpty()) {
            RelativePath binaryPath(config.getTargetBinaryRelativePathString(), RelativePath::projectFolder);
            outputDir = binaryPath.rebased(projectFolder, getTargetFolder(), RelativePath::buildTargetFolder).toUnixStyle();
        }

        out << "    DESTDIR = " << addQuotesIfContainsSpaces(outputDir) << newLine;
        out << "    OBJECTS_DIR = " << addQuotesIfContainsSpaces(intermediatesDirName) << newLine;
    }

    void writeTarget(OutputStream& out, const BuildConfiguration& config) const {
        String targetName(replacePreprocessorTokens(config, config.getTargetBinaryNameString()));
        if (projectType.isStaticLibrary() || projectType.isDynamicLibrary())
            targetName = getLibbedFilename(targetName);
        else
            targetName = targetName.upToLastOccurrenceOf(".", false, false) + makefileTargetSuffix;
        out << "    TARGET = " << addQuotesIfContainsSpaces(targetName) << newLine;
    }

    void writeDefineFlags(OutputStream& out, const BuildConfiguration& config) const {

#if JUCE_LINUX
        out << "    linux:DEFINES += LINUX=1" << newLine;
#elif JUCE_WINDOWS
        out << "    win32:DEFINES += _WINDOWS=1 JUCE_MSVC=1" << newLine;
#elif JUCE_MAC
    //TODO: check defines on MAC
#endif

        StringPairArray defines;
        if (config.isDebug()) {
            defines.set("DEBUG", "1");
            defines.set("_DEBUG", "1");
        } else {
            defines.set("NDEBUG", "1");
        }

        out << "    DEFINES +=";
        out << " JUCE_QT_CREATOR=1";//BCC: special qt define
        const StringPairArray& defs = mergePreprocessorDefs(defines, getAllPreprocessorDefs(config));
        for (int i = 0; i < defs.size(); ++i) {
            out << " " << defs.getAllKeys()[i];
            const String value(defs.getAllValues()[i]);
            if (value.isNotEmpty())
                out << "=" << value;
        }
        out << newLine;
    }

    void writeCxxFlags(OutputStream& out, const BuildConfiguration& config) const {

#if JUCE_MAC
        // macos flags
        String macArch("-arch x86_64");
        String macCflags = macArch + String(config.isDebug() ? " -O0" : " -O3") + " "
                + static_cast<const QtBuildConfiguration&>(config).getWarningLevelVar().toString();

        out << "    macx:QMAKE_CXXFLAGS += " << macCflags << newLine;
        out << "    macx:QMAKE_OBJECTIVE_CFLAGS += " << macCflags << newLine;
        out << "    macx:QMAKE_LFLAGS += " << macArch;

        StringArray macFrameworks;
        macFrameworks.addTokens("Accelerate AudioUnit AudioToolbox Carbon Cocoa CoreAudio CoreAudioKit CoreMIDI DiscRecording IOKit OpenGL QTKit QuartzCore WebKit", true);//QuickTime
        if (macFrameworks.size() > 0) {
            for (String frame: macFrameworks) {
                out << " -framework " << frame;
            }
        }
        out << newLine;

#elif JUCE_LINUX
        // linux flags
        out << "    linux:QMAKE_CXXFLAGS += -Wreorder -Wuninitialized -Wunused-parameter -Wstrict-aliasing -Wshadow -Wsign-compare" << newLine;
        out << "    linux:QMAKE_CXXFLAGS +=";

        if (config.isDebug())
            out << " -g -ggdb";

        if (makefileIsDLL)
            out << " -fPIC";

        out << " -O" << config.getGCCOptimisationFlag()
            << " " << (replacePreprocessorTokens(config, getExtraCompilerFlagsString())).trimEnd();

        String cppStandardToUse(getCppStandardString());
        if (cppStandardToUse.isEmpty())
            cppStandardToUse = "-std=c++11";
        out << cppStandardToUse;

        StringArray flags(makefileExtraLinkerFlags);

        if (makefileIsDLL)
            flags.add("-shared");

        if (!config.isDebug())
            flags.add("-fvisibility=hidden");

        if (flags.size() > 0) {
            out << " " << getCleanedStringArray(flags).joinIntoString(" ");
        }

        out << " " << replacePreprocessorTokens(config, getExtraLinkerFlagsString()).trim();
        out << newLine;

#elif JUCE_WINDOWS
        // TODO: check win flags

#endif // JUCE_MAC JUCE_LINUX JUCE_WINDOWS

    }

    void writeHeaderPathFlags(OutputStream& out, const BuildConfiguration& config) const {
        StringArray searchPaths(extraSearchPaths);
        searchPaths.addArray(config.getHeaderSearchPaths());
        //        searchPaths.insert(0, "/usr/include/freetype2");
        //        searchPaths.insert(0, "/usr/include");
        searchPaths = getCleanedStringArray(searchPaths);

        out << "    INCLUDEPATH += \\" << newLine;
        for (int i = 0; i < searchPaths.size(); ++i) {
            out << "        " << addQuotesIfContainsSpaces(FileHelpers::unixStylePath(replacePreprocessorTokens(config, searchPaths[i]))) //BCC: escapeSpaces
                << " \\"  << newLine;
        }
        out << newLine << newLine;

#if JUCE_MAC
        //TODO: check mac includepaths

#elif JUCE_LINUX
        out << "    linux:INCLUDEPATH += /usr/include /usr/include/freetype2" << newLine;

#elif JUCE_WINDOWS
        out << "    win32:INCLUDEPATH += \"C:/Program Files (x86)/Windows Kits/10/Include/10.0.10240.0/ucrt\"" << newLine;

#endif // JUCE_MAC JUCE_LINUX JUCE_WINDOWS

    }

    void writeLibs(OutputStream& out, const BuildConfiguration& config) const {

        out << "    LIBS +=" << " \\" << newLine;

        // add library search paths -L
        const StringArray librarySearchPaths (config.getLibrarySearchPaths());
        for (int i = 0; i < librarySearchPaths.size(); ++i) {
            out << "        -L" << librarySearchPaths[i] << " \\" << newLine;
        }

        // add libraries -l
        StringArray libraries;
        libraries.addTokens(getExternalLibrariesString(), ";", "\"'");
        libraries.removeEmptyStrings();
        if (libraries.size() != 0) {
            out << "        -l" << replacePreprocessorTokens(config, libraries.joinIntoString(" -l")).trim() << " \\" << newLine;
        }
        out << newLine << newLine;

#if JUCE_MAC
        // TODO: check mac libs

#elif JUCE_LINUX
        out << "    linux:LIBS +=";

        out << config.getGCCLibraryPathFlags();
        for (int i = 0; i < linuxLibs.size(); ++i) {
            out << " -l" << linuxLibs[i];
        }

        if (getProject().isConfigFlagEnabled("JUCE_USE_CURL")) {
            out << " -lcurl";
        }

#elif JUCE_WINDOWS
        out << "    win32:LIBS += -lkernel32 -luser32 -lgdi32 -lwinspool -lcomdlg32 -ladvapi32 -lshell32 -lole32 -loleaut32 -luuid -lodbc32 -lodbccp32";
        out << newLine;
        //TODO: get this path from environment variable
        // bug with "corecrt.h" and "ucrtd.lib"
        // https://bugreports.qt.io/browse/QTBUG-50191?focusedCommentId=307268&page=com.atlassian.jira.plugin.system.issuetabpanels:comment-tabpanel#comment-307268
        out << "    win32:LIBS += -L\"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.10240.0/ucrt/x64\" -lvcruntimed -lucrtd";
        out << newLine;

#endif // JUCE_MAC JUCE_LINUX JUCE_WINDOWS

    }

    void writeConfig(OutputStream& out, const BuildConfiguration& config) const {

        out << "CONFIG(" << escapeSpaces(config.getName()).toLowerCase() << ", debug|release) {" << newLine;

        writeOutputDir(out, config);
        writeTarget(out, config);
        writeDefineFlags(out, config);
        writeHeaderPathFlags(out, config);
        writeCxxFlags(out, config);
        writeLibs(out, config);

        out << "}" << newLine << newLine;
    }

    bool shouldFileBeCompiledByMac (const RelativePath& file) const
    {
        return file.hasFileExtension (objCFileExtensions);
    }

    void findAllFilesToCompile(const Project::Item& projectItem, Array<RelativePath>& results) const {
        if (projectItem.isGroup()) {
            for (int i = 0; i < projectItem.getNumChildren(); ++i)
                findAllFilesToCompile(projectItem.getChild(i), results);

        } else {
            RelativePath relPath(projectItem.getFile(), getTargetFolder(), RelativePath::buildTargetFolder);
            if (projectItem.shouldBeCompiled())
            {
                results.add(relPath);
            }
        }
    }

    void findAllResourceFiles(const Project::Item& projectItem, Array<RelativePath>& results) const {
        if (projectItem.isGroup()) {
            for (int i = 0; i < projectItem.getNumChildren(); ++i)
                findAllResourceFiles(projectItem.getChild(i), results);

        } else {
            RelativePath relPath(projectItem.getFile(), getTargetFolder(), RelativePath::buildTargetFolder);
            if (!projectItem.shouldBeCompiled() || projectItem.shouldBeAddedToBinaryResources())
            {
                results.add(relPath);
            }
        }
    }

    void writeSources(OutputStream& out) const {
        Array<RelativePath> files;
        for (int i = 0; i < getAllGroups().size(); ++i) {
            findAllFilesToCompile(getAllGroups().getReference(i), files);
        }

#if JUCE_MAC
        // mac mm files
        out << "macx {" << newLine;
        out << "OBJECTIVE_SOURCES += \\" << newLine;
        for (int i = 0; i < files.size(); ++i) {
            if (shouldFileBeCompiledByMac(files.getReference(i))) {
                jassert(files.getReference(i).getRoot() == RelativePath::buildTargetFolder);
                out << "    " << addQuotesIfContainsSpaces(files.getReference(i).toUnixStyle()) << " \\" << newLine;
            }
        }
        out << newLine << "}" << newLine << newLine;
#endif // JUCE_MAC

        out << "SOURCES += \\" << newLine;
        for (int i = 0; i < files.size(); ++i) {
            if (shouldFileBeCompiledByDefault(files.getReference(i))) {
                jassert(files.getReference(i).getRoot() == RelativePath::buildTargetFolder);
                out << "    " << addQuotesIfContainsSpaces(files.getReference(i).toUnixStyle()) << " \\" << newLine;
            }
        }
        out << newLine << newLine;
    }

    void writeResources(OutputStream& out) const {
        Array<RelativePath> files;
        for (int i = 0; i < getAllGroups().size(); ++i) {
            findAllResourceFiles(getAllGroups().getReference(i), files);
        }

        out << "OTHER_FILES += \\" << newLine;

        for (int i = 0; i < files.size(); ++i) {
            if (!files.getReference(i).hasFileExtension(headerFileExtensions)) {
                jassert(files.getReference(i).getRoot() == RelativePath::buildTargetFolder);
                out << "    " << addQuotesIfContainsSpaces(files.getReference(i).toUnixStyle()) << " \\" << newLine;
            }
        }

        out << newLine << newLine;
    }

    void findAllHeaderFiles(const Project::Item& projectItem, Array<RelativePath>& results) const {
        if (projectItem.isGroup()) {
            for (int i = 0; i < projectItem.getNumChildren(); ++i)
                findAllHeaderFiles(projectItem.getChild(i), results);
        } else {
            results.add(RelativePath(projectItem.getFile(), getTargetFolder(), RelativePath::buildTargetFolder));
        }
    }

    void writeHeaders(OutputStream& out) const {
        Array<RelativePath> files;
        const Array<Project::Item>& groups = getAllGroups();
        for (int i = 0; i < groups.size(); ++i) {
            findAllHeaderFiles(groups.getReference(i), files);
        }

        out << "HEADERS += \\" << newLine;
        for (int i = 0; i < files.size(); ++i) {
            if (files.getReference(i).hasFileExtension(headerFileExtensions)) {
                jassert(files.getReference(i).getRoot() == RelativePath::buildTargetFolder);
                out << "    " << addQuotesIfContainsSpaces(files.getReference(i).toUnixStyle()) << " \\" << newLine;
            }
        }
        out << newLine << newLine;
    }

    void writeGlobalConfig(OutputStream& out) const
    {
        out << "CONFIG -= qt" << newLine;
        out << "CONFIG -= debug_and_release" << newLine;
        //TODO: if (getCppStandardString() == "-std=c++11")
        out << "CONFIG += c++11" << newLine;
#if JUCE_MAC
        //TODO: check if mac sdk 10.11
        out << "QMAKE_MAC_SDK = macosx10.11" << newLine;
#endif
        //NOTE: makes posible to have "dir1/file.cpp" and "dir2/file.cpp"
        //out << "CONFIG += object_parallel_to_source" << newLine;

        //TODO: use this:
        //projectType.isAudioPlugin()
        //projectType.isGUIApplication() // nothing to do, or use TEMPLATE

        if (projectType.isCommandLineApp()) {
            // The target is a Win32 console application
            out << newLine;
            out << "QT += core" << newLine;
            out << "QT -= gui" << newLine;
            out << "CONFIG += console" << newLine;
            out << "CONFIG -= app_bundle" << newLine;
            out << "TEMPLATE = app" << newLine;
            out << "DEFINES += _AFXDLL JUCER_VS2012_78A501F" << newLine;

        } else if (projectType.isStaticLibrary()) {
            //TODO: not tested
            out << "TEMPLATE = lib" << newLine;
            out << "CONFIG += static" << newLine;

        } else if (projectType.isDynamicLibrary()) {
            //TODO: not tested
            out << "TEMPLATE = lib" << newLine;
            out << "CONFIG += shared" << newLine;
        }

        out << newLine;
    }

    void writeQtProject(OutputStream& out) const {
        out << "# Automatically generated qt project file, created by the Bccjucer" << newLine
            << "# Don't edit this file! Your changes will be overwritten when you re-save the Bccjucer project!" << newLine
            << newLine;

        writeGlobalConfig(out);

        for (ConstConfigIterator config(*this); config.next();) {
            writeConfig(out, *config);
        }

        writeSources(out);
        writeHeaders(out);
        writeResources(out);

        out << newLine;
    }

    JUCE_DECLARE_NON_COPYABLE(QtCreatorProjectExporter)
};
