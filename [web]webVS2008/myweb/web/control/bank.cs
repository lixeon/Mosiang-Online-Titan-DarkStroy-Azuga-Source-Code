namespace web.control
{
    using System;
    using System.Web.UI;
    using System.Web.UI.WebControls;
    using web;

    public class bank : UserControl
    {
        protected Button btnedit;
        protected DropDownList ddchalist;
        protected RadioButton rbget;
        protected RadioButton rbput;
        protected RegularExpressionValidator RegularExpressionValidator1;
        protected RequiredFieldValidator RequiredFieldValidator1;
        protected RequiredFieldValidator Requiredfieldvalidator6;
        protected TextBox tbmoney;
        protected string webbank;

        private void btnedit_Click(object sender, EventArgs e)
        {
            double money = Math.Abs(double.Parse(this.tbmoney.Text.ToString()));
            int useridx = int.Parse(base.Session["useridx"].ToString());
            int chaidxs = int.Parse(this.ddchalist.SelectedValue.ToString());
            int opertype = this.rbget.Checked ? 1 : 0;
            string str = new WebLogic().bank(base.Session["userid"].ToString(), useridx, chaidxs, money, opertype);
            base.Response.Write("<script language=javascript>alert('" + str + "')</script>");
        }

        private void InitializeComponent()
        {
            this.btnedit.Click += new EventHandler(this.btnedit_Click);
            base.Load += new EventHandler(this.Page_Load);
        }

        protected override void OnInit(EventArgs e)
        {
            this.InitializeComponent();
            base.OnInit(e);
        }

        private void Page_Load(object sender, EventArgs e)
        {
            if (base.Session["userid"] == null)
            {
                base.Response.Redirect("login.aspx?returnurl=" + base.Request.ServerVariables["SCRIPT_NAME"]);
            }
            else if (!this.Page.IsPostBack)
            {
                this.webbank = new DataProviders().ExecScalarOne("select webbank from mhcmember..chr_log_info where propid=" + base.Session["useridx"].ToString());
                this.ddchalist.DataSource = new DataProviders().ExecuteSqlDs("select character_idx,character_name+'[瞷Τ'+convert(varchar,character_money)+'じ]' as character_name from mhgame..tb_character  where substring(character_name,1,1)!='@' and user_idx=" + base.Session["useridx"].ToString(), "ddchalist");
                this.ddchalist.DataTextField = "character_name";
                this.ddchalist.DataValueField = "character_idx";
                this.ddchalist.DataBind();
                for (int i = 0; i < this.ddchalist.Items.Count; i++)
                {
                    this.ddchalist.Items[i].Text = new system().ConvertToBig5(this.ddchalist.Items[i].Text.ToString(), 950);
                }
            }
        }
    }
}
