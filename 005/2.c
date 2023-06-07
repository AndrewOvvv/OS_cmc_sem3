int 
my_strlen(char *str)
{
    int len = 0;
    while (str[len] != 0) {
        ++len;
    }
    return len;
}


void
normalize_path(char *buf)
{
    int len = my_strlen(buf);
    int write_position = 0, was_directory = 1;
    for (int i = 0; i < len; ++i) {
        if (buf[i] == '/' && was_directory) {
            buf[write_position] = buf[i];
            ++write_position;
            was_directory = 0;
        } else if (buf[i] != '/') {
            was_directory = 0;
            int right = i, cnt_dots = (buf[i] == '.');
            while (right + 1 < len && buf[right + 1] != '/') {
                ++right;
                if (buf[right] == '.') {
                    ++cnt_dots;
                }
            }
            if (right - i == 1 &&  cnt_dots == 2) {
                int cnt_slashes = 0;
                while (write_position - 1 > 0 && (cnt_slashes < 1 || buf[write_position - 1] != '/')) {
                    --write_position;
                    if (buf[write_position] == '/') {
                        ++cnt_slashes;
                    }
                    buf[write_position] = 0;
                }
            } else if (right - i != 0 || cnt_dots != 1) {
                for (int j = i; j < right + 1; ++j) {
                    buf[write_position] = buf[j];
                    ++write_position;
                }
                was_directory = 1;
            }
            i = right;
        }
    }
    int new_len = write_position; 
    if (buf[new_len - 1] == '/' && new_len != 1) {
        buf[new_len - 1] = 0;
    }
    for (int i = new_len; i < len; ++i) {
        buf[i] = 0;
    } 
}
