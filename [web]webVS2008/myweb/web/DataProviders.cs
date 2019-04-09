namespace web
{
    using System;
    using System.Data;
    using System.Data.SqlClient;
    using System.Runtime.InteropServices;
    using System.Web;

    public class DataProviders
    {
        protected SqlConnection myConn;
        protected string mySql;
        protected string strConn = HttpContext.Current.Application["db.connect"].ToString();

        public DataProviders()
        {
            this.myConn = new SqlConnection(this.strConn);
        }

        public SqlCommand BuildIntCommand(string ProcName, IDataParameter[] parameters)
        {
            SqlCommand command = this.BuildQueryCommand(ProcName, parameters);
            command.Parameters.Add(new SqlParameter("ReturnValue", SqlDbType.Int, 4, ParameterDirection.ReturnValue, false, 0, 0, string.Empty, DataRowVersion.Default, null));
            return command;
        }

        public SqlCommand BuildQueryCommand(string ProcName, IDataParameter[] parameters)
        {
            SqlCommand command = new SqlCommand(ProcName, this.myConn);
            command.CommandType = CommandType.StoredProcedure;
            foreach (SqlParameter parameter in parameters)
            {
                command.Parameters.Add(parameter);
            }
            return command;
        }

        public void CloseConn()
        {
            if (this.myConn.State == ConnectionState.Open)
            {
                this.myConn.Close();
            }
        }

        public int ExecScalar(SqlCommand scmd)
        {
            int num;
            scmd.Connection = this.myConn;
            try
            {
                this.myConn.Open();
                SqlDataReader reader = scmd.ExecuteReader();
                if (reader.Read())
                {
                    reader.Close();
                    return 1;
                }
                reader.Close();
                num = 0;
            }
            catch (Exception exception)
            {
                throw exception;
            }
            finally
            {
                scmd.Dispose();
                this.CloseConn();
            }
            return num;
        }

        public int ExecScalar(string mySql)
        {
            int num;
            SqlCommand command = new SqlCommand(mySql, this.myConn);
            try
            {
                this.myConn.Open();
                if (command.ExecuteReader().Read())
                {
                    return 1;
                }
                num = 0;
            }
            catch (Exception exception)
            {
                throw exception;
            }
            finally
            {
                command.Dispose();
                this.CloseConn();
            }
            return num;
        }

        public string ExecScalarOne(string mySql)
        {
            string str2;
            SqlCommand command = new SqlCommand(mySql, this.myConn);
            try
            {
                this.myConn.Open();
                SqlDataReader reader = command.ExecuteReader();
                if (reader.Read())
                {
                    string str = reader[0].ToString();
                    reader.Close();
                    return str;
                }
                reader.Close();
                str2 = "none";
            }
            catch (Exception exception)
            {
                throw exception;
            }
            finally
            {
                command.Dispose();
                this.CloseConn();
            }
            return str2;
        }

        public int ExecuteCount(string mySql)
        {
            int num;
            SqlCommand command = new SqlCommand(mySql, this.myConn);
            command.CommandText = mySql;
            try
            {
                this.myConn.Open();
                num = (int) command.ExecuteScalar();
            }
            catch (Exception exception)
            {
                throw exception;
            }
            finally
            {
                command.Dispose();
                this.myConn.Close();
            }
            return num;
        }

        public int ExecuteSql(SqlCommand scmd)
        {
            int num;
            scmd.Connection = this.myConn;
            try
            {
                this.myConn.Open();
                num = scmd.ExecuteNonQuery();
            }
            catch (Exception exception)
            {
                throw exception;
            }
            finally
            {
                scmd.Dispose();
                this.CloseConn();
            }
            return num;
        }

        public int ExecuteSql(string mySql)
        {
            int num;
            SqlCommand command = new SqlCommand(mySql, this.myConn);
            try
            {
                this.myConn.Open();
                num = command.ExecuteNonQuery();
            }
            catch (Exception exception)
            {
                throw exception;
            }
            finally
            {
                command.Dispose();
                this.CloseConn();
            }
            return num;
        }

        public object ExecuteSql4ValueEx(string mySql)
        {
            object obj3;
            SqlCommand command = new SqlCommand(mySql, this.myConn);
            try
            {
                this.myConn.Open();
                object objA = command.ExecuteScalar();
                if (object.Equals(objA, null))
                {
                    throw new Exception("object unavailable!");
                }
                obj3 = objA;
            }
            catch (Exception exception)
            {
                throw exception;
            }
            finally
            {
                command.Dispose();
                this.myConn.Close();
            }
            return obj3;
        }

        public DataSet ExecuteSqlChannelReapter(string mySql1, string mySql2, string myTable1, string myTable2)
        {
            DataSet set2;
            SqlCommand selectCommand = new SqlCommand(mySql1, this.myConn);
            SqlCommand command2 = new SqlCommand(mySql2, this.myConn);
            SqlDataAdapter adapter = new SqlDataAdapter(selectCommand);
            SqlDataAdapter adapter2 = new SqlDataAdapter(command2);
            DataSet dataSet = new DataSet();
            try
            {
                adapter.Fill(dataSet, myTable1);
                adapter2.Fill(dataSet, myTable2);
                set2 = dataSet;
            }
            catch (Exception exception)
            {
                throw exception;
            }
            finally
            {
                adapter.Dispose();
                adapter2.Dispose();
            }
            return set2;
        }

        public SqlDataReader ExecuteSqlDataReader(string mySql)
        {
            SqlDataReader reader2;
            SqlCommand command = new SqlCommand(mySql, this.myConn);
            try
            {
                this.myConn.Open();
                reader2 = command.ExecuteReader(CommandBehavior.CloseConnection);
            }
            catch (Exception exception)
            {
                throw exception;
            }
            finally
            {
                command.Dispose();
            }
            return reader2;
        }

        public DataSet ExecuteSqlDs(SqlCommand scmd, string myTable)
        {
            DataSet set2;
            scmd.Connection = this.myConn;
            SqlDataAdapter adapter = new SqlDataAdapter();
            adapter.SelectCommand = scmd;
            DataSet dataSet = new DataSet();
            try
            {
                this.myConn.Open();
                adapter.Fill(dataSet, myTable);
                set2 = dataSet;
            }
            catch (Exception exception)
            {
                throw exception;
            }
            finally
            {
                this.CloseConn();
            }
            return set2;
        }

        public DataSet ExecuteSqlDs(string mySql, string myTable)
        {
            DataSet set2;
            SqlDataAdapter adapter = new SqlDataAdapter(mySql, this.myConn);
            DataSet dataSet = new DataSet();
            try
            {
                adapter.Fill(dataSet, myTable);
                set2 = dataSet;
            }
            catch (Exception exception)
            {
                throw exception;
            }
            finally
            {
                dataSet.Dispose();
                adapter.Dispose();
                this.CloseConn();
            }
            return set2;
        }

        public DataSet ExecuteSqlDsReapter(string mySql, int repeatestr1, int repeatestr2, string myTable)
        {
            DataSet set2;
            SqlCommand selectCommand = new SqlCommand(mySql, this.myConn);
            SqlDataAdapter adapter = new SqlDataAdapter(selectCommand);
            DataSet dataSet = new DataSet();
            try
            {
                adapter.Fill(dataSet, repeatestr1, repeatestr2, myTable);
                set2 = dataSet;
            }
            catch (Exception exception)
            {
                throw exception;
            }
            finally
            {
                adapter.Dispose();
            }
            return set2;
        }

        public SqlDataReader RunProcedure(string ProcName, params SqlParameter[] parameters)
        {
            this.myConn.Open();
            SqlCommand command = this.BuildQueryCommand(ProcName, parameters);
            command.CommandType = CommandType.StoredProcedure;
            return command.ExecuteReader(CommandBehavior.CloseConnection);
        }

        public DataSet RunProcedure(string ProcName, IDataParameter[] parameters, string tableName)
        {
            DataSet dataSet = new DataSet();
            SqlDataAdapter adapter = new SqlDataAdapter();
            adapter.SelectCommand = this.BuildQueryCommand(ProcName, parameters);
            adapter.Fill(dataSet, tableName);
            this.myConn.Close();
            return dataSet;
        }

        public int RunProcedure(string ProcName, IDataParameter[] parameters, out int rowsAffected)
        {
            this.myConn.Open();
            SqlCommand command = this.BuildIntCommand(ProcName, parameters);
            rowsAffected = command.ExecuteNonQuery();
            int num = (int) command.Parameters["ReturnValue"].Value;
            command.Dispose();
            this.myConn.Close();
            return num;
        }

        public void RunProcedure(string ProcName, IDataParameter[] parameters, DataSet myDs, string tableName)
        {
            this.myConn.Open();
            SqlDataAdapter adapter = new SqlDataAdapter();
            adapter.SelectCommand = this.BuildIntCommand(ProcName, parameters);
            adapter.Fill(myDs, tableName);
            this.myConn.Close();
        }

        public string sear(string str)
        {
            return str.Replace("'", "\"");
        }

        public string str(string str)
        {
            return ("'" + str.Replace("'", "\"") + "'");
        }
    }
}
