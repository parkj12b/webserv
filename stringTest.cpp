int HeaderLine::pushValue()
{
    std::vector<std::string>::iterator  vectorIt;
    std::string::iterator               stringIt;
    std::string                         str;
    bool                                commaError;
    bool                                comma;
    int                                 index;
    int                                 count;

    comma = true;
    commaError = false;
    index = 0;
    count = 0;
    LOG(std::cout<<"here\n"<<std::endl);
    for (stringIt = value.begin(); stringIt != value.end(); stringIt++)
    {
        if (*stringIt == '\"')
            comma = !comma;
        else if (comma && *stringIt == ',')
        {
            str = value.substr(index, count);
            index = count + 1;
            eraseSpace(str, false);
            if (str.empty())
                continue ;
            header[key].push_back(str);
            commaError = true;
        }
        count++;
    }
    if (comma)
    {
        if (value.find(',') == std::string::npos)
        {
            eraseSpace(value, false);
            if (!value.empty())
            {
                header[key].push_back(str);
                commaError = true;
            }
        }
    }
    LOG(std::cout<<"here2\n"<<std::endl);
    if (!comma || !commaError)
        return (-2);
    vectorIt = std::find(manyHeader.begin(), manyHeader.end(), key);
    LOG(std::cout<<"here3\n"<<std::endl);
    if (vectorIt == manyHeader.end())
    {
        if (header[key].size() > 1)
            return (-2);
    }
    LOG(std::cout<<"here4\n"<<std::endl);
    return (0);
}