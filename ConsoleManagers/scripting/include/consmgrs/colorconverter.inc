stock char[] ConvertColorToHTML(int color[3])
{
    char colorHTML[32] = "000000";
    int Quotient = color[0] * 65536 + color[1] * 256 + color[2];
    int Remainder;

    for(int i = 5; i >= 0; i--)
    {
        Remainder = Quotient % 16;
        Quotient = Quotient / 16;

        if(Remainder == 0) colorHTML[i] = '0';
        else if(Remainder == 1) colorHTML[i] = '1';
        else if(Remainder == 2) colorHTML[i] = '2';
        else if(Remainder == 3) colorHTML[i] = '3';
        else if(Remainder == 4) colorHTML[i] = '4';
        else if(Remainder == 5) colorHTML[i] = '5';
        else if(Remainder == 6) colorHTML[i] = '6';
        else if(Remainder == 7) colorHTML[i] = '7';
        else if(Remainder == 8) colorHTML[i] = '8';
        else if(Remainder == 9) colorHTML[i] = '9';
        else if(Remainder == 10) colorHTML[i] = 'A';
        else if(Remainder == 11) colorHTML[i] = 'B';
        else if(Remainder == 12) colorHTML[i] = 'C';
        else if(Remainder == 13) colorHTML[i] = 'D';
        else if(Remainder == 14) colorHTML[i] = 'E';
        else if(Remainder == 15) colorHTML[i] = 'F';

        if(Quotient == 0) break;
    }
    return colorHTML;
}

stock void ConvertHTMLToColor(const char[] htmltag, int phrase)
{
    int cvrtHexToDec[6];

    for(int i = 0; i <= 5; i++)
    {
        if(htmltag[i + 1] == '0') cvrtHexToDec[i] = 0;
        else if(htmltag[i + 1] == '1') cvrtHexToDec[i] = 1;
        else if(htmltag[i + 1] == '2') cvrtHexToDec[i] = 2;
        else if(htmltag[i + 1] == '3') cvrtHexToDec[i] = 3;
        else if(htmltag[i + 1] == '4') cvrtHexToDec[i] = 4;
        else if(htmltag[i + 1] == '5') cvrtHexToDec[i] = 5;
        else if(htmltag[i + 1] == '6') cvrtHexToDec[i] = 6;
        else if(htmltag[i + 1] == '7') cvrtHexToDec[i] = 7;
        else if(htmltag[i + 1] == '8') cvrtHexToDec[i] = 8;
        else if(htmltag[i + 1] == '9') cvrtHexToDec[i] = 9;
        else if(htmltag[i + 1] == 'A' || htmltag[i + 1] == 'a') cvrtHexToDec[i] = 10;
        else if(htmltag[i + 1] == 'B' || htmltag[i + 1] == 'b') cvrtHexToDec[i] = 11;
        else if(htmltag[i + 1] == 'C' || htmltag[i + 1] == 'c') cvrtHexToDec[i] = 12;
        else if(htmltag[i + 1] == 'D' || htmltag[i + 1] == 'd') cvrtHexToDec[i] = 13;
        else if(htmltag[i + 1] == 'E' || htmltag[i + 1] == 'e') cvrtHexToDec[i] = 14;
        else if(htmltag[i + 1] == 'F' || htmltag[i + 1] == 'f') cvrtHexToDec[i] = 15;
    }

    configsDetail[phrase].RGBsColor[0] = cvrtHexToDec[0] * 16 + cvrtHexToDec[1];
    configsDetail[phrase].RGBsColor[1] = cvrtHexToDec[2] * 16 + cvrtHexToDec[3];
    configsDetail[phrase].RGBsColor[2] = cvrtHexToDec[4] * 16 + cvrtHexToDec[5];
}