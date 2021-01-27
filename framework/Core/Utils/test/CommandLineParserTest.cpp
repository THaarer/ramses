//  -------------------------------------------------------------------------
//  Copyright (C) 2014 BMW Car IT GmbH
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------

#include "framework_common_gmock_header.h"
#include "gtest/gtest.h"
#include <Utils/CommandLineParser.h>
#include "Utils/Argument.h"

namespace ramses_internal
{
    TEST(CommandLineParserTest, NoArguments)
    {
        const Char* params[] = { "myExe" };
        CommandLineParser parser(1, params);

        StringVector files = parser.getNonOptions();
        EXPECT_TRUE(files.empty());
    }

    TEST(CommandLineParserTest, GetProgramName)
    {
        const Char* params[] = { "myExe" };
        CommandLineParser parser(1, params);

        EXPECT_STREQ("myExe", parser.getProgramName().c_str());
    }

    TEST(CommandLineParserTest, shortStringArg)
    {
        const Char* params[] = { "myExe", "-a", "argForA", "-x", "argForB" };
        CommandLineParser parser(5, params);

        EXPECT_EQ(String("argForA"), String(ArgumentString(parser, "a", "longParamA", "default")));
        EXPECT_EQ(String("argForB"), String(ArgumentString(parser, "x", "longParamB", "defaultB")));

        EXPECT_EQ(String("default"), String(ArgumentString(parser, "", "a", "default")));

        EXPECT_TRUE(parser.getNonOptions().empty());
    }

    TEST(CommandLineParserTest, stripsWhiteSpacesFromArgumentsWithDashPrefix)
    {
        const Char* params[] = { "myExe", "-i ", " ipAddressWhichShouldBeParsedProperlyButNotTrimmed "};
        CommandLineParser parser(3, params);

        EXPECT_EQ(String(" ipAddressWhichShouldBeParsedProperlyButNotTrimmed "), String(ArgumentString(parser, "i", "ipAddress", "default")));

        EXPECT_TRUE(parser.getNonOptions().empty());
    }

    TEST(CommandLineParserTest, doesNotTrimArgumentsWithQuotes)
    {
        const Char* params[] = { "myExe", "--kevinSpacey ", " iWantTheseSpaces_HandsOff " };
        CommandLineParser parser(3, params);

        EXPECT_EQ(String(" iWantTheseSpaces_HandsOff "), String(ArgumentString(parser, "k", "kevinSpacey", "HouseOfCards")));

        EXPECT_TRUE(parser.getNonOptions().empty());
    }

    TEST(CommandLineParserTest, LongStringArg)
    {
        const Char* params[] = { "myExe", "--a", "argForA", "--x", "argForB" };
        CommandLineParser parser(5, params);

        EXPECT_EQ(String("argForA"), String(ArgumentString(parser, "ashort", "a", "default")));
        EXPECT_EQ(String("argForB"), String(ArgumentString(parser, "xShort", "x", "defaultB")));

        EXPECT_TRUE(parser.getNonOptions().empty());
    }

    TEST(CommandLineParserTest, BoolSwitchArguments)
    {
        const Char* params[] = { "myExe", "--a", "-b", "--x" };
        CommandLineParser parser(4, params);

        EXPECT_TRUE(ArgumentBool(parser, "", "a"));
        EXPECT_TRUE(ArgumentBool(parser, "b", "longParamB"));
        EXPECT_TRUE(ArgumentBool(parser, "", "x"));

        EXPECT_TRUE(parser.getNonOptions().empty());
    }

    TEST(CommandLineParserTest, onlyFileTarget)
    {
        const Char* params[] = { "myExe", "--v", "-ip", "192.168.1.1", "my/test/scene" };
        CommandLineParser parser(5, params);

        EXPECT_TRUE(ArgumentBool(parser, "", "v"));
        EXPECT_EQ(String("192.168.1.1"), String(ArgumentString(parser, "ip", "daemon-ip", "localhost")));
        StringVector files = parser.getNonOptions();
        EXPECT_TRUE(contains_c(files, "my/test/scene"));
    }

    TEST(CommandLineParserTest, fileTargetAtTheEnd)
    {
        const Char* params[] = { "myExe", "--a", "something", "--b", "anothertest", "my/test/scene" };
        CommandLineParser parser(6, params);

        ArgumentString valueA(parser, "", "a", "default");
        UNUSED(valueA);
        ArgumentString valueB(parser, "", "b", "default");
        UNUSED(valueB);

        StringVector files = parser.getNonOptions();
        EXPECT_TRUE(contains_c(files, "my/test/scene"));
        EXPECT_EQ(1U, files.size());
    }

    TEST(CommandLineParserTest, multipleFileTargets)
    {
        const Char* params[] = { "myExe", "--a", "something", "--b", "anothertest", "my/test/scene", "otherFile" };
        CommandLineParser parser(7, params);

        ArgumentString valueA(parser, "", "a", "default");
        UNUSED(valueA);
        ArgumentString valueB(parser, "", "b", "default");
        UNUSED(valueB);

        StringVector files = parser.getNonOptions();
        EXPECT_TRUE(contains_c(files, "my/test/scene"));
        EXPECT_TRUE(contains_c(files, "otherFile"));
        EXPECT_EQ(2U, files.size());
    }

    TEST(CommandLineParserTest, BoolOptionFollowedByFile)
    {
        const Char* params[] = { "myExe", "--b", "file" };
        CommandLineParser parser(3, params);

        ArgumentBool value(parser, "", "b");
        UNUSED(value);

        StringVector files = parser.getNonOptions();
        EXPECT_EQ(1U, files.size());
    }

    TEST(CommandLineParserTest, BoolOption)
    {
        const Char* params[] = { "myExe", "--b", "file" };
        CommandLineParser parser(3, params);

        ArgumentString value(parser, "", "b", "defaultValue");
        UNUSED(value);

        StringVector files = parser.getNonOptions();
        EXPECT_EQ(0U, files.size());
    }

    TEST(CommandLineParserTest, MixedArgumentsAndFileTargets)
    {
        const Char* params[] = { "myExe", "testfileOne", "-shortOption", "secondFile", "--longString", "stringArgument", "--longFloat", "2345.45", "--longOptioN", "thirdfile" };
        CommandLineParser parser(10, params);

        EXPECT_EQ(String("stringArgument"), String(ArgumentString(parser, "", "longString", "defaultValue")));
        Float val = ArgumentFloat(parser, "", "longFloat", 0.0f);
        EXPECT_FLOAT_EQ(2345.45f, val);
        EXPECT_TRUE(ArgumentBool(parser, "shortOption", ""));
        EXPECT_TRUE(ArgumentBool(parser, "", "longOptioN"));

        StringVector files = parser.getNonOptions();
        EXPECT_TRUE(contains_c(files, "testfileOne"));
        EXPECT_TRUE(contains_c(files, "secondFile"));
        EXPECT_TRUE(contains_c(files, "thirdfile"));
        EXPECT_EQ(3U, files.size());
    }

    TEST(CommandLineParserTest, NegativeFloatArguments)
    {
        const Char* params[] = { "myExe", "-x", "-1.25", "-y", "4.5", "-z", "-1x", "-w", "-69" };
        CommandLineParser parser(9, params);

        Float valX = ArgumentFloat(parser, "x", "", 0.0f);
        EXPECT_FLOAT_EQ(-1.25, valX);

        Float valY = ArgumentFloat(parser, "y", "", 0.0f);
        EXPECT_FLOAT_EQ(4.5, valY);

        Float valZ = ArgumentFloat(parser, "z", "", 0.0f);
        EXPECT_FLOAT_EQ(0.0, valZ);

        EXPECT_TRUE(ArgumentBool(parser, "1x", ""));

        Float valW = ArgumentFloat(parser, "w", "", 0.0f);
        EXPECT_FLOAT_EQ(-69, valW);

        EXPECT_TRUE(parser.getNonOptions().empty());
    }

    TEST(CommandLineParserTest, IntegerArguments)
    {
        const Char* params[] = { "myExe", "-x", "-1", "-y", "4", "-z", "-w", "-69" };
        CommandLineParser parser(8, params);

        const Int32 valX = ArgumentInt32(parser, "x", "", 0);
        EXPECT_EQ(-1, valX);

        const Int32 valY = ArgumentInt32(parser, "y", "", 0);
        EXPECT_EQ(4, valY);

        const Int32 valZ = ArgumentInt32(parser, "z", "", 0);
        EXPECT_EQ(0, valZ);

        const Int32 valW = ArgumentInt32(parser, "w", "", 0);
        EXPECT_EQ(-69, valW);

        EXPECT_TRUE(parser.getNonOptions().empty());
    }

    TEST(CommandLineParserTest, UInt16Arguments)
    {
        const Char* params[] = { "myExe", "-x", "-1", "-y", "4", "-z", "-w", "69", "-a", "65535" };
        CommandLineParser parser(10, params);

        const UInt16 valX = ArgumentUInt16(parser, "x", "", 0);
        EXPECT_EQ(0u, valX);

        const UInt16 valY = ArgumentUInt16(parser, "y", "", 0);
        EXPECT_EQ(4u, valY);

        const UInt16 valZ = ArgumentUInt16(parser, "z", "", 0);
        EXPECT_EQ(0u, valZ);

        const UInt16 valW = ArgumentUInt16(parser, "w", "", 0);
        EXPECT_EQ(69u, valW);

        const UInt16 valA = ArgumentUInt16(parser, "a", "", 0);
        EXPECT_EQ(65535u, valA);

        EXPECT_TRUE(parser.getNonOptions().empty());
    }

    TEST(CommandLineParserTest, UInt32Arguments)
    {
        const Char* params[] = { "myExe", "-x", "-1", "-y", "4", "-z", "-w", "69", "-a", "4294967295" };
        CommandLineParser parser(10, params);

        const UInt32 valX = ArgumentUInt32(parser, "x", "", 0);
        EXPECT_EQ(0u, valX);

        const UInt32 valY = ArgumentUInt32(parser, "y", "", 0);
        EXPECT_EQ(4u, valY);

        const UInt32 valZ = ArgumentUInt32(parser, "z", "", 0);
        EXPECT_EQ(0u, valZ);

        const UInt32 valW = ArgumentUInt32(parser, "w", "", 0);
        EXPECT_EQ(69u, valW);

        const UInt32 valA = ArgumentUInt32(parser, "a", "", 0);
        EXPECT_EQ(4294967295u, valA);

        EXPECT_TRUE(parser.getNonOptions().empty());
    }

    TEST(CommandLineParserTest, Vec3Arguments)
    {
        const Char* params[] = { "myExe", "-correct", "[5,6,7]", "-noValue", "-overflow", "[6,7,8,9]", "-withoutParentheses", "5,6,7", "-wrong", "[5,6]", "-verywrong", "[5,6,7" };
        CommandLineParser parser(12, params);

        const Vector3 defaultValue(1, 2, 3);
        const Vector3 correct           = ArgumentVec3(parser, "correct", "", defaultValue);
        const Vector3 noValue           = ArgumentVec3(parser, "noValue", "", defaultValue);
        const Vector3 overflow          = ArgumentVec3(parser, "overflow", "", defaultValue);
        const Vector3 withoutParentheses= ArgumentVec3(parser, "withoutParentheses", "", defaultValue);
        const Vector3 wrong             = ArgumentVec3(parser, "wrong", "", defaultValue);
        const Vector3 verywrong         = ArgumentVec3(parser, "verywrong", "", defaultValue);

        EXPECT_EQ(Vector3(5, 6, 7), correct);
        EXPECT_EQ(defaultValue, noValue);
        EXPECT_EQ(defaultValue, overflow);
        EXPECT_EQ(defaultValue, withoutParentheses);
        EXPECT_EQ(defaultValue, wrong);
        EXPECT_EQ(defaultValue, verywrong);


        EXPECT_TRUE(parser.getNonOptions().empty());
    }

    TEST(CommandLineParserTest, DelayedParsingArgumentBool)
    {
        const Char* params[] = { "myExe", "-x"};
        CommandLineParser parser(2, params);

        ArgumentBool valX("x", "");
        EXPECT_FALSE(valX); //default value, not parsed yet

        bool parsedVal = valX.parseFromCmdLine(parser);
        EXPECT_TRUE(parsedVal);

        EXPECT_TRUE(parser.getNonOptions().empty());
    }

    TEST(CommandLineParserTest, DelayedParsingArgumentString)
    {
        const Char* params[] = { "myExe", "-x", "abc" };
        CommandLineParser parser(3, params);

        Argument<String> valX("x", "", "default");
        EXPECT_EQ(String("default"), valX); //default value, not parsed yet

        String parsedVal = valX.parseValueFromCmdLine(parser);
        EXPECT_EQ(String("abc"), parsedVal);

        EXPECT_TRUE(parser.getNonOptions().empty());
    }

    TEST(CommandLineParserTest, DelayedParsingArgumentFloat)
    {
        const Char* params[] = { "myExe", "-x", "0.1" };
        CommandLineParser parser(3, params);

        Argument<Float> valX("x", "", 0.9f);
        EXPECT_FLOAT_EQ(0.9f, valX); //default value, not parsed yet

        Float parsedVal = valX.parseValueFromCmdLine(parser);
        EXPECT_FLOAT_EQ(0.1f, parsedVal);

        EXPECT_TRUE(parser.getNonOptions().empty());
    }

    TEST(CommandLineParserTest, DelayedParsingArgumentInt32)
    {
        const Char* params[] = { "myExe", "-x", "20" };
        CommandLineParser parser(3, params);

        Argument<Int32> valX("x", "", 10);
        EXPECT_EQ(10, valX); //default value, not parsed yet

        Int32 parsedVal = valX.parseValueFromCmdLine(parser);
        EXPECT_EQ(20, parsedVal);

        EXPECT_TRUE(parser.getNonOptions().empty());
    }

    TEST(CommandLineParserTest, DelayedParsingArgumentUInt16)
    {
        const Char* params[] = { "myExe", "-x", "20" };
        CommandLineParser parser(3, params);

        Argument<UInt16> valX("x", "", 10u);
        EXPECT_EQ(10u, valX); //default value, not parsed yet

        UInt16 parsedVal = valX.parseValueFromCmdLine(parser);
        EXPECT_EQ(20u, parsedVal);

        EXPECT_TRUE(parser.getNonOptions().empty());
    }

    TEST(CommandLineParserTest, DelayedParsingArgumentUInt32)
    {
        const Char* params[] = { "myExe", "-x", "20" };
        CommandLineParser parser(3, params);

        Argument<UInt32> valX("x", "", 10u);
        EXPECT_EQ(10u, valX); //default value, not parsed yet

        UInt32 parsedVal = valX.parseValueFromCmdLine(parser);
        EXPECT_EQ(20u, parsedVal);

        EXPECT_TRUE(parser.getNonOptions().empty());
    }

    TEST(CommandLineArgumentsTest, ArgumentProvidesGetHelpStringMethod)
    {
        Argument<String> valX("xx", "longx", "myDefault", "myDescription");
        const std::string result = valX.getHelpString();

        const auto posShortName = result.find("xx");
        EXPECT_NE(std::string::npos, posShortName);

        const auto posLongName = result.find("longx");
        EXPECT_NE(std::string::npos, posLongName);
        EXPECT_GT(posLongName, posShortName);

        const auto posValueMarker = result.find("<value>");
        EXPECT_NE(std::string::npos, posValueMarker);
        EXPECT_GT(posValueMarker, posLongName);

        const auto posDescription = result.find("myDescription");
        EXPECT_NE(std::string::npos, posDescription);
        EXPECT_GT(posDescription, posValueMarker);

        const auto posDefault = result.find("myDefault");
        EXPECT_NE(std::string::npos, posDefault);
        EXPECT_GT(posDefault, posDescription);
    }

    TEST(CommandLineArgumentsTest, ArgumentBoolProvidesGetHelpStringMethod)
    {
        ArgumentBool valX("xx", "longx", "myDescription");
        const std::string result = valX.getHelpString();

        const auto posShortName = result.find("xx");
        EXPECT_NE(std::string::npos, posShortName);

        const auto posLongName = result.find("longx");
        EXPECT_NE(std::string::npos, posLongName);
        EXPECT_GT(posLongName, posShortName);

        const auto posValueMarker = result.find("<value>");
        EXPECT_EQ(std::string::npos, posValueMarker); //no value marker for bool arguments

        const auto posDescription = result.find("myDescription");
        EXPECT_NE(std::string::npos, posDescription);
        EXPECT_GT(posDescription, posLongName);

        const auto posDefault = result.find("false");
        EXPECT_NE(std::string::npos, posDefault);
        EXPECT_GT(posDefault, posDescription);
    }

    TEST(CommandLineParserTest, OptionCanBeSetSeveralTimes)
    {
        std::vector<const Char*> params({ "myExe", "-s", "ABCDE", "-s", "zzZZzzZZ" });
        CommandLineParser parser(params.size(), params.data());

        ArgumentString val("s", "", "xyz");
        EXPECT_EQ("xyz", String(val)); //default value, not parsed yet

        const String parsedVal = val.parseValueFromCmdLine(parser);
        EXPECT_EQ("ABCDE", parsedVal);

        EXPECT_EQ("ABCDE", String(val));
        EXPECT_TRUE(val.next());
        EXPECT_EQ("zzZZzzZZ", String(val));
        EXPECT_FALSE(val.next());
    }

    TEST(CommandLineParserTest, OptionCanParsedSeveralTimes)
    {

        ArgumentString val("s", "", "xyz");
        std::vector<const Char*> params({ "myExe", "-s", "ABCDE", "-s", "zzZZzzZZ" });
        CommandLineParser parser(params.size(), params.data());

        const String parsedVal = val.parseValueFromCmdLine(parser);
        EXPECT_EQ("ABCDE", parsedVal);
        EXPECT_EQ("ABCDE", String(val));
        EXPECT_TRUE(val.next());
        EXPECT_EQ("zzZZzzZZ", String(val));
        EXPECT_FALSE(val.next());

        std::vector<const Char*> params2({ "myExe", "-s", "xyz", "-s", "qwerty" });
        CommandLineParser parser2(params2.size(), params2.data());

        const String parsedVal2 = val.parseValueFromCmdLine(parser2);
        EXPECT_EQ("xyz", parsedVal2);
        EXPECT_EQ("xyz", String(val));
        EXPECT_TRUE(val.next());
        EXPECT_EQ("qwerty", String(val));
        EXPECT_FALSE(val.next());
    }

    TEST(CommandLineParserTest, OptionCanBeSetSeveralTimes_DoesNotChangeValueIfNoMoreCanBeFound)
    {
        std::vector<const Char*> params({ "myExe", "-s", "ABCDE", "-s", "zzZZzzZZ" });
        CommandLineParser parser(params.size(), params.data());

        ArgumentString val("s", "", "xyz");
        EXPECT_EQ("xyz", String(val)); //default value, not parsed yet

        const String parsedVal = val.parseValueFromCmdLine(parser);
        EXPECT_EQ("ABCDE", parsedVal);

        EXPECT_EQ("ABCDE", String(val));
        EXPECT_TRUE(val.next());
        EXPECT_EQ("zzZZzzZZ", String(val));
        EXPECT_FALSE(val.next());

        //no more changes
        EXPECT_EQ("zzZZzzZZ", String(val));
        EXPECT_FALSE(val.next());
    }

    TEST(CommandLineParserTest, OptionCanBeSetSeveralTimes_BoolOptionIsSetToTrue)
    {
        const Char* params[] = { "", "-a", "argForA", "--longParam" };
        CommandLineParser parser(3, params);

        EXPECT_TRUE(ArgumentBool(parser, "a", "longParamA", "default").wasDefined());
    }

    TEST(CommandLineParserTest, OptionCanBeSetSeveralTimes_WithouMixingOptions)
    {
        std::vector<const Char*> params({ "myExe", "-x", "20", "-x", "100", "-y", "1.0", "-y", "3.0", "-y", "9.0" });
        CommandLineParser parser(params.size(), params.data());

        ArgumentUInt16 valX("x", "", 10u);
        ArgumentFloat  valY("y", "", 10.f);
        EXPECT_EQ(10u, valX); //default value, not parsed yet
        EXPECT_FLOAT_EQ(10.f, valY); //default value, not parsed yet

        const UInt16 parsedVal = valX.parseValueFromCmdLine(parser);
        EXPECT_EQ(20u, parsedVal);

        EXPECT_EQ(20u, valX);
        EXPECT_TRUE(valX.next());
        EXPECT_EQ(100u, valX);
        EXPECT_FALSE(valX.next());

        const Float parsedValF = valY.parseValueFromCmdLine(parser);
        EXPECT_FLOAT_EQ(1.f, parsedValF);

        EXPECT_FLOAT_EQ(1.f, valY);
        EXPECT_TRUE(valY.next());
        EXPECT_FLOAT_EQ(3.f, valY);
        EXPECT_TRUE(valY.next());
        EXPECT_FLOAT_EQ(9.f, valY);
        EXPECT_FALSE(valY.next());
    }

    TEST(CommandLineParserTest, OptionCanBeSetSeveralTimes_WithMixingOptions)
    {
        std::vector<const Char*> params({ "myExe", "-x", "20", "-y", "1.0", "-z", "abcd", "-y", "3.0", "-x", "100", "-y", "9.0" });
        CommandLineParser parser(params.size(), params.data());

        ArgumentUInt16 valX("x", "", 10u);
        ArgumentFloat  valY("y", "", 10.f);
        ArgumentString valZ("z", "", "ZZzzzZZ");
        EXPECT_EQ(10u, valX); //default value, not parsed yet
        EXPECT_FLOAT_EQ(10.f, valY); //default value, not parsed yet
        EXPECT_EQ("ZZzzzZZ", String(valZ)); //default value, not parsed yet

        const UInt16 parsedVal = valX.parseValueFromCmdLine(parser);
        EXPECT_EQ(20u, parsedVal);

        EXPECT_EQ(20u, valX);
        EXPECT_TRUE(valX.next());
        EXPECT_EQ(100u, valX);
        EXPECT_FALSE(valX.next());

        const String parsedValString = valZ.parseValueFromCmdLine(parser);
        EXPECT_EQ("abcd", parsedValString);
        EXPECT_FALSE(valZ.next());

        const Float parsedValF = valY.parseValueFromCmdLine(parser);
        EXPECT_FLOAT_EQ(1.f, parsedValF);

        EXPECT_FLOAT_EQ(1.f, valY);
        EXPECT_TRUE(valY.next());
        EXPECT_FLOAT_EQ(3.f, valY);
        EXPECT_TRUE(valY.next());
        EXPECT_FLOAT_EQ(9.f, valY);
        EXPECT_FALSE(valY.next());
    }

    TEST(CommandLineParserTest, OptionCanBeSetSeveralTimes_WithMixingOptions_AccessInDifferentOrder)
    {
        std::vector<const Char*> params({ "myExe", "-x", "20", "-y", "1.0", "-z", "abcd", "-y", "3.0", "-x", "100", "-y", "9.0" });
        CommandLineParser parser(params.size(), params.data());

        ArgumentUInt16 valX("x", "", 10u);
        ArgumentFloat  valY("y", "", 10.f);
        ArgumentString valZ("z", "", "ZZzzzZZ");
        EXPECT_EQ(10u, valX); //default value, not parsed yet
        EXPECT_FLOAT_EQ(10.f, valY); //default value, not parsed yet
        EXPECT_EQ("ZZzzzZZ", String(valZ)); //default value, not parsed yet

        const UInt16 parsedVal = valX.parseValueFromCmdLine(parser);
        const String parsedValString = valZ.parseValueFromCmdLine(parser);
        const Float parsedValF = valY.parseValueFromCmdLine(parser);

        EXPECT_EQ(20u, parsedVal);
        EXPECT_EQ("abcd", parsedValString);
        EXPECT_FLOAT_EQ(1.f, parsedValF);

        EXPECT_FLOAT_EQ(1.f, valY);
        EXPECT_TRUE(valY.next());

        EXPECT_EQ(20u, valX);
        EXPECT_TRUE(valX.next());

        EXPECT_FLOAT_EQ(3.f, valY);
        EXPECT_TRUE(valY.next());

        EXPECT_EQ(100u, valX);
        EXPECT_FALSE(valX.next());

        EXPECT_FLOAT_EQ(9.f, valY);
        EXPECT_FALSE(valY.next());

        EXPECT_FALSE(valZ.next());
    }

    TEST(CommandLineParserTest, OptionCanBeSetSeveralTimes_IgnoresErroneousOptionsWithNoValueSet)
    {
        std::vector<const Char*> params({ "myExe", "-s", "ABCDE", "-s", "-s", "zzZZzzZZ" });
        CommandLineParser parser(params.size(), params.data());

        ArgumentString val("s", "", "xyz");
        EXPECT_EQ("xyz", String(val)); //default value, not parsed yet

        const String parsedVal = val.parseValueFromCmdLine(parser);
        EXPECT_EQ("ABCDE", parsedVal);

        EXPECT_EQ("ABCDE", String(val));
        EXPECT_TRUE(val.next());

        //gets the next set value correctly
        EXPECT_EQ("zzZZzzZZ", String(val));
        EXPECT_FALSE(val.next());
    }
}
