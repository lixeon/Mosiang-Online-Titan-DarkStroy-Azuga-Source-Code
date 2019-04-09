namespace web.admin
{
    using System;
    using System.Data;
    using System.Web.UI;
    using System.Web.UI.WebControls;
    using web;

    public class cploginlog : Page
    {
        protected Button Button1;
        protected DataGrid DataGrid2;
        private static DataSet ds;
        protected static int rcount;
        private static string sql;
        protected TextBox tbscreatetimeend;
        protected TextBox tbscreatetimestart;
        protected TextBox tbsuserid;
        protected TextBox tbswebregip;

        private void Button1_Click(object sender, EventArgs e)
        {
            system system = new system();
            string str = system.ChkSql(this.tbsuserid.Text.ToString().Trim());
            string str2 = system.ChkSql(this.tbswebregip.Text.ToString().Trim());
            string str3 = system.ChkSql(this.tbscreatetimestart.Text.ToString().Trim());
            string str4 = system.ChkSql(this.tbscreatetimeend.Text.ToString().Trim());
            sql = "select * from mhcmember..login_log  where";
            string str5 = "";
            int num = 0;
            if (str != "")
            {
                if (num == 0)
                {
                    str5 = str5 + " login_id like '%" + str + "%'";
                    num++;
                }
                else
                {
                    str5 = str5 + " and login_id like '%" + str + "%'";
                }
            }
            if (str2 != "")
            {
                if (num == 0)
                {
                    str5 = str5 + " ip like '%" + str2 + "%'";
                    num++;
                }
                else
                {
                    str5 = str5 + " and ip like '%" + str2 + "%'";
                }
            }
            if (str3 != "")
            {
                if (num == 0)
                {
                    str5 = str5 + " start_date >= '" + str3 + "'";
                    num++;
                }
                else
                {
                    str5 = str5 + " and start_date >= '" + str3 + "'";
                }
            }
            if (str4 != "")
            {
                if (num == 0)
                {
                    str5 = str5 + " start_date <= '" + str4 + "'";
                }
                else
                {
                    str5 = str5 + " and start_date <= '" + str4 + "'";
                }
            }
            if (num == 0)
            {
                sql = sql.Replace("where", "");
            }
            sql = sql + str5;
            ds = new DataProviders().ExecuteSqlDs(sql, "DataGrid1");
            this.DataGrid2.DataSource = ds;
            this.DataGrid2.CurrentPageIndex = 0;
            this.DataGrid2.DataBind();
            rcount = ds.Tables[0].Rows.Count;
        }

        private void InitializeComponent()
        {
            this.Button1.Click += new EventHandler(this.Button1_Click);
            base.Load += new EventHandler(this.Page_Load);
        }

        protected override void OnInit(EventArgs e)
        {
            this.InitializeComponent();
            base.OnInit(e);
        }

        private void Page_Load(object sender, EventArgs e)
        {
        }
    }
}
