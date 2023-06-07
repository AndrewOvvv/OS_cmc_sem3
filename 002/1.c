STYPE
bit_reverse(STYPE value)
{
    UTYPE type_size = 1;
    UTYPE current = 1; 
    while ((UTYPE)(current << 1) > 0) {
        current = (UTYPE)(current << 1);
        type_size++;
    }
        
    current = (UTYPE)value;
    UTYPE result = 0;    

    for (int from_position = 0; from_position < type_size; ++from_position) {
        int to_position = type_size - from_position - 1;
        if ((current & (UTYPE)((UTYPE)(1) << from_position)) != 0) {
            result = (result | (UTYPE)((UTYPE)(1) << to_position));
        }
    }
    return result;
}
