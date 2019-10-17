#ifndef COM_FUNC_H
#define COM_FUNC_H

#define MAXLINE 512

#define SERV_TCP_PORT_READ 59004
#define SERV_TCP_PORT_WRITE 59003

//#define SERV_HOST_ADDR "147.87.149.40" /* RoboGuide PC Robot */
#define SERV_HOST_ADDR "147.87.144.251" /* Robot */




/**
 * @brief connectReadJoint
 * @param current_joint_pos
 * @return
 */
float* connectReadJoint (float *current_joint_pos);

/**
 * @brief connectReadCartesian
 * @param current_cartesian_pos
 * @return
 */
float *connectReadCartesian (float *current_cartesian_pos);

/**
 * @brief connectWriteJoint
 * @param goto_joint_pos
 * @param current_joint_pos
 */
void connectWriteJoint (float *goto_joint_pos, float *current_joint_pos);

/**
 * @brief connectWriteCartesian
 * @param goto_cartesian_pos
 * @param current_cartesian_pos
 */
void connectWriteCartesian (float *goto_cartesian_pos, float *current_cartesian_pos);

/**
 * @brief readJointPos
 * @param sockfd
 * @return
 */
float* readPos(int sockfd);

/**
 * @brief writeJointPos
 * @param sockfd
 * @param goto_pos
 */
void writePos (int sockfd, float *goto_pos);

/**
 * @brief readline
 * @param fd
 * @param ptr
 * @param maxlen
 * @param input
 * @return
 */
float readline(int fd, char *ptr, int maxlen, char input[MAXLINE+1]);

/**
 * @brief written
 * @param fd
 * @param ptr
 * @param nbytes
 * @return
 */
int written(int fd, char *ptr, int nbytes);

/**
 * @brief gripperOn
 */
void gripperOn (void);

/**
 * @brief gripperOff
 */
void gripperOff (void);

#endif // COM_FUNC_H
